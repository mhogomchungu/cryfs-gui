/*
 *
 *  Copyright (c) 2012-2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keydialog.h"
#include "ui_keydialog.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QDir>
#include <QTableWidget>
#include <QDebug>
#include <QFile>

#include "options.h"
#include "dialogmsg.h"
#include "task.h"
#include "utility.h"
#include "lxqt_wallet/frontend/lxqt_wallet.h"

#define KWALLET         "kde wallet"
#define INTERNAL_WALLET "internal wallet"
#define GNOME_WALLET    "gnome wallet"

/*
 * this ugly global variable is defined in zulucrypt.cpp to prevent multiple prompts when opening multiple volumes
 */
static QString _internalPassWord ;

keyDialog::keyDialog( QWidget * parent,QTableWidget * table,const volumeInfo& e,std::function< void() > p,std::function< void( const QString& ) > q ) :
	QDialog( parent ),m_ui( new Ui::keyDialog ),m_cancel( std::move( p ) ),m_success( std::move( q ) )
{
	m_ui->setupUi( this ) ;

	m_ui->checkBoxShareMountPoint->setVisible( false ) ;

	m_table = table ;
	m_path = e.volumeName() ;
	m_working = false ;

	decltype( tr( "" ) ) msg ;

	m_create = e.volumeName().isEmpty() ;

	m_reUseMountPoint = utility::reUseMountPoint() ;

	if( m_create ){

		m_ui->pbOpen->setText( tr( "&Create" ) ) ;

		m_ui->label_2->setText( tr( "Volume Name" ) ) ;

		m_ui->label_3->setVisible( true ) ;

		m_ui->checkBoxOpenReadOnly->setVisible( false ) ;

		m_ui->lineEditFolderPath->setVisible( true ) ;

		m_ui->pbOpenFolderPath->setVisible( true ) ;

		m_ui->pbMountPoint->setVisible( false ) ;
		m_ui->lineEditFolderPath->setText( utility::homePath() + "/" ) ;

		m_ui->lineEditFolderPath->setEnabled( false ) ;

		m_ui->lineEditMountPoint->setFocus() ;

		m_ui->lineEditMountPoint->setText( m_path ) ;

		msg = tr( "Create A New Cryfs Volume" ) ;
	}else{
		msg = tr( "Unlocking \"%1\"" ).arg( m_path ) ;

		m_ui->lineEditMountPoint->setEnabled( true ) ;

		m_ui->label_2->setText( tr( "Mount Path" ) ) ;

		m_ui->label_3->setVisible( false ) ;

		m_ui->lineEditFolderPath->setVisible( false ) ;

		m_ui->checkBoxOpenReadOnly->setVisible( true ) ;

		m_ui->pbOpenFolderPath->setVisible( false ) ;

		m_ui->pbMountPoint->setIcon( QIcon( ":/folder.png" ) ) ;

		m_ui->lineEditKey->setFocus() ;

		m_ui->lineEditMountPoint->setText( [ & ](){

			auto m = e.mountPoint() ;

			if( m.startsWith( "/" ) ){

				if( m_reUseMountPoint ){

					return m ;
				}else{
					auto y = m ;
					auto r = y.lastIndexOf( '/' ) ;

					if( r != -1 ){

						y.truncate( r ) ;
					}

					return y + "/" + utility::mountPathPostFix( m,m.split( '/' ).last() ) ;
				}
			}else{
				if( m_reUseMountPoint ){

					if( m.isEmpty() ){

						return utility::mountPath( m_path.split( "/" ).last() ) ;
					}else{
						return utility::mountPath( m.split( "/" ).last() ) ;
					}
				}else{
					return utility::mountPath( [ &m,this ](){

						if( m.isEmpty() ){

							return utility::mountPathPostFix( m_path.split( "/" ).last() ) ;
						}else{
							return utility::mountPathPostFix( m ) ;
						}
					}() ) ;
				}
			}
		}() ) ;
	}

	this->setWindowTitle( msg ) ;

	m_ui->pbOpenFolderPath->setIcon( QIcon( ":/folder.png" ) ) ;

	m_menu = new QMenu( this ) ;

	connect( m_menu,SIGNAL( triggered( QAction * ) ),this,SLOT( pbPluginEntryClicked( QAction * ) ) ) ;

	this->setFixedSize( this->size() ) ;
	this->setWindowFlags( Qt::Window | Qt::Dialog ) ;
	this->setFont( parent->font() ) ;

	m_ui->checkBoxOpenReadOnly->setChecked( utility::getOpenVolumeReadOnlyOption() ) ;

	m_ui->lineEditKey->setEchoMode( QLineEdit::Password ) ;

	connect( m_ui->pbCancel,SIGNAL( clicked() ),this,SLOT( pbCancel() ) ) ;
	connect( m_ui->pbOpen,SIGNAL( clicked() ),this,SLOT( pbOpen() ) ) ;
	connect( m_ui->pbkeyOption,SIGNAL( clicked() ),this,SLOT( pbkeyOption() ) ) ;
	connect( m_ui->pbOpenFolderPath,SIGNAL( clicked() ),this,SLOT( pbFolderPath() ) ) ;
	connect( m_ui->pbMountPoint,SIGNAL( clicked() ),this,SLOT( pbMountPointPath() ) ) ;
	connect( m_ui->checkBoxOpenReadOnly,SIGNAL( stateChanged( int ) ),this,SLOT( cbMountReadOnlyStateChanged( int ) ) ) ;
	connect( m_ui->cbKeyType,SIGNAL( currentIndexChanged( int ) ),this,SLOT( cbActicated( int ) ) ) ;
	connect( m_ui->lineEditMountPoint,SIGNAL( textChanged( QString ) ),this,SLOT( textChanged( QString ) ) ) ;

	m_ui->cbKeyType->addItem( tr( "Key" ) ) ;
	m_ui->cbKeyType->addItem( tr( "KeyFile" ) ) ;
	m_ui->cbKeyType->addItem( tr( "Key+KeyFile" ) ) ;
	m_ui->cbKeyType->addItem( tr( "Plugin" ) ) ;

	m_ui->checkBoxShareMountPoint->setEnabled( false ) ;

	if( m_create ){

		m_ui->lineEditMountPoint->setFocus() ;
	}else{
		m_ui->lineEditKey->setFocus() ;
	}

	this->installEventFilter( this ) ;

	m_ui->pbOptions->setMenu( [ this ](){

		auto m = new QMenu( this ) ;

		connect( m->addAction( tr( "Set Idle Timeout" ) ),
			 SIGNAL( triggered() ),this,SLOT( mountOptions() ) ) ;

                connect( m->addAction( tr( "Set Cryfs Configuration File Path" ) ),
                         SIGNAL( triggered() ),this,SLOT( configFile() ) ) ;
		return m ;
	}() ) ;
}

void keyDialog::mountOptions()
{
        auto e = tr( "Automatically Unmount After Specified Minutes of Inactivity Is Reached." ) ;

        options::instance( this,false,e,[ this ]( const QString& e ){

		m_options = e ;
	} ) ;
}

void keyDialog::configFile()
{
        auto e = tr( "Unlock A Cryfs Volume With Specified Configuration File." ) ;

        options::instance( this,true,e,[ this ]( const QString& e ){

                m_configFile = e ;
        } ) ;
}

bool keyDialog::eventFilter( QObject * watched,QEvent * event )
{
	return utility::eventFilter( this,watched,event,[ this ](){ this->pbCancel() ; } ) ;
}

void keyDialog::cbMountReadOnlyStateChanged( int state )
{
	auto e = utility::setOpenVolumeReadOnly( this,state == Qt::Checked ) ;

	m_ui->checkBoxOpenReadOnly->setEnabled( false ) ;
	m_ui->checkBoxOpenReadOnly->setChecked( e ) ;
	m_ui->checkBoxOpenReadOnly->setEnabled( true ) ;

	if( m_ui->lineEditKey->text().isEmpty() ){

		m_ui->lineEditKey->setFocus() ;

	}else if( m_ui->lineEditMountPoint->text().isEmpty() ){

		m_ui->lineEditMountPoint->setFocus() ;
	}else{
		m_ui->pbOpen->setFocus() ;
	}
}

void keyDialog::textChanged( QString e )
{
	auto r =  m_ui->lineEditFolderPath->text() ;

	auto l = r.lastIndexOf( '/' ) ;

	if( l != -1 ){

		r.truncate( l + 1 ) ;

		m_ui->lineEditFolderPath->setText( r + e ) ;
	}
}

void keyDialog::pbMountPointPath()
{
	auto msg = tr( "Select A Folder To Create A Mount Point In." ) ;
	auto e = QFileDialog::getExistingDirectory( this,msg,utility::homePath(),QFileDialog::ShowDirsOnly ) ;

	if( !e.isEmpty() ){

		e = e + "/" + m_ui->lineEditMountPoint->text().split( '/' ).last() ;

		m_ui->lineEditMountPoint->setText( e ) ;
	}
}

void keyDialog::pbFolderPath()
{
	auto msg = tr( "Select A Folder To Create A Mount Point In." ) ;
	auto e = QFileDialog::getExistingDirectory( this,msg,utility::homePath(),QFileDialog::ShowDirsOnly ) ;

	if( !e.isEmpty() ){

		e = e + "/" + m_ui->lineEditFolderPath->text().split( '/' ).last() ;

		m_ui->lineEditFolderPath->setText( e ) ;
	}
}

void keyDialog::enableAll()
{
	m_ui->pbMountPoint->setEnabled( true ) ;
        m_ui->pbOptions->setEnabled( true ) ;
	m_ui->label_2->setEnabled( true ) ;
	m_ui->lineEditMountPoint->setEnabled( !m_create ) ;
	m_ui->pbOpenFolderPath->setEnabled( true ) ;
	m_ui->pbCancel->setEnabled( true ) ;
	m_ui->pbOpen->setEnabled( true ) ;
	m_ui->label->setEnabled( true ) ;
	m_ui->cbKeyType->setEnabled( true ) ;

	m_ui->lineEditKey->setEnabled( m_ui->cbKeyType->currentIndex() == keyDialog::Key ) ;

	m_ui->pbkeyOption->setEnabled( true ) ;
	m_ui->checkBoxOpenReadOnly->setEnabled( true ) ;

	m_ui->checkBoxShareMountPoint->setEnabled( false ) ;
	m_ui->lineEditFolderPath->setEnabled( false ) ;
	m_ui->label_3->setEnabled( true ) ;
}

void keyDialog::disableAll()
{
	m_ui->pbMountPoint->setEnabled( false ) ;
	m_ui->cbKeyType->setEnabled( false ) ;
	m_ui->pbOptions->setEnabled( false ) ;
	m_ui->pbkeyOption->setEnabled( false ) ;
	m_ui->label_2->setEnabled( false ) ;
	m_ui->label_3->setEnabled( false ) ;
	m_ui->lineEditMountPoint->setEnabled( false ) ;
	m_ui->pbOpenFolderPath->setEnabled( false ) ;
	m_ui->lineEditKey->setEnabled( false ) ;
	m_ui->pbCancel->setEnabled( false ) ;
	m_ui->pbOpen->setEnabled( false ) ;
	m_ui->label->setEnabled( false ) ;
	m_ui->checkBoxOpenReadOnly->setEnabled( false ) ;
	m_ui->checkBoxShareMountPoint->setEnabled( false ) ;
	m_ui->lineEditFolderPath->setEnabled( false ) ;
}

void keyDialog::KeyFile()
{
	if( m_ui->cbKeyType->currentIndex() == keyDialog::keyfile ){

		auto msg = tr( "Select A File To Be Used As A Keyfile." ) ;
		auto e = QFileDialog::getOpenFileName( this,msg,utility::homePath() ) ;

		if( !e.isEmpty() ){

			m_ui->lineEditKey->setText( e ) ;
		}
	}
}

void keyDialog::pbkeyOption()
{
	auto keyType = m_ui->cbKeyType->currentIndex() ;

	if( keyType == keyDialog::plugin ){

		this->Plugin() ;

	}else if( keyType == keyDialog::keyfile ){

		this->KeyFile() ;
	}
}

void keyDialog::Plugin()
{
	utility::createPlugInMenu( m_menu,tr( INTERNAL_WALLET ),tr( GNOME_WALLET ),tr( KWALLET ) ) ;

	m_menu->setFont( this->font() ) ;

	m_menu->addSeparator() ;

	m_menu->addAction( tr( "Cancel" ) ) ;

	m_menu->exec( QCursor::pos() ) ;
}

void keyDialog::pbPluginEntryClicked( QAction * ac )
{
	auto e = ac->text() ;

	e.remove( "&" ) ;

	if( e != tr( "Cancel" ) ){

		m_ui->lineEditKey->setText( e ) ;
	}
}

void keyDialog::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->pbCancel() ;
}

void keyDialog::pbOpen()
{
	if( m_create ){

		if( m_ui->lineEditMountPoint->text().isEmpty() ){

			DialogMsg msg( this ) ;

			return msg.ShowUIOK( tr( "ERROR" ),tr( "Volume Name Field Is Empty." ) ) ;
		}

		if( m_ui->lineEditKey->text().isEmpty() ){

			DialogMsg msg( this ) ;

			return msg.ShowUIOK( tr( "ERROR" ),tr( "Key Field Is Empty." ) ) ;
		}
	}

	this->disableAll() ;

	if( m_ui->cbKeyType->currentIndex() == keyDialog::plugin ){

		utility::wallet w ;

		auto wallet = m_ui->lineEditKey->text() ;

		if( wallet == tr( KWALLET ) ){

			w = utility::getKeyFromWallet( this,LxQt::Wallet::kwalletBackEnd,m_path ) ;

		}else if( wallet == tr( INTERNAL_WALLET ) ){

			w = utility::getKeyFromWallet( this,LxQt::Wallet::internalBackEnd,m_path,_internalPassWord ) ;

			if( w.notConfigured ){

				DialogMsg msg( this ) ;
				msg.ShowUIOK( tr( "ERROR!" ),tr( "Internal Wallet Is Not Configured." ) ) ;
				return this->enableAll() ;
			}else{
				_internalPassWord = w.password ;
			}

		}else if( wallet == tr( GNOME_WALLET ) ){

			w = utility::getKeyFromWallet( this,LxQt::Wallet::secretServiceBackEnd,m_path ) ;
		}else{
			return this->openVolume() ;
		}

		if( w.opened ){

			if( w.key.isEmpty() ){

				DialogMsg msg( this ) ;

				msg.ShowUIOK( tr( "ERROR" ),tr( "The Volume Does Not Appear To Have An Entry In The Wallet." ) ) ;

				this->enableAll() ;

				if( m_ui->cbKeyType->currentIndex() != keyDialog::Key ){

					m_ui->lineEditKey->setEnabled( false ) ;
				}
			}else{
				m_key = w.key ;
				this->openVolume() ;
			}
		}else{
			_internalPassWord.clear() ;
			this->enableAll() ;
		}
	}else{
		this->openVolume() ;
	}
}

bool keyDialog::completed( cryfsTask::status s )
{
	DialogMsg msg( this ) ;

	switch( s ){

	case cryfsTask::status::success :

		return true ;

	case cryfsTask::status::cryfs :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Unlock A Cryfs Volume.\nWrong Password Entered." ) ) ;
		break;

	case cryfsTask::status::encfs :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Unlock An Encfs Volume.\nWrong Password Entered." ) ) ;
		break;

	case cryfsTask::status::cryfsNotFound :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Complete The Request.\nCryfs Executable Could Not Be Found." ) ) ;
		break;

	case cryfsTask::status::encfsNotFound :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Complete The Request.\nEncfs Executable Could Not Be Found." ) ) ;
		break;

	case cryfsTask::status::failedToCreateMountPoint :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Create Mount Point." ) ) ;
		break;

	case cryfsTask::status::unknown :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Unlock The Volume.\nNot Supported Volume Encountered." ) ) ;
		break;

	case cryfsTask::status::backendFail :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Complete The Task.\nBackend Not Responding." ) ) ;
		break;
	default:
		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Complete The Task.\nAn Unknown Error Has Occured." ) ) ;
		break;
	}

	return false ;
}

void keyDialog::encryptedFolderCreate()
{
	auto path = m_ui->lineEditFolderPath->text() ;

	auto m = path.split( '/' ).last() ;

	if( utility::pathExists( path ) ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),tr( "Encrypted Folder Path Is Already Taken." ) ) ;

		return this->enableAll() ;
	}

	m = utility::mountPath( utility::mountPathPostFix( m ) ) ;

	if( utility::pathExists( m ) && !m_reUseMountPoint ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),tr( "Mount Point Path Already Taken." ) ) ;

		return this->enableAll() ;
	}

        auto& e = cryfsTask::encryptedFolderCreate( { path,m,m_key,m_options,"",false,m_success } ) ;

	if( this->completed( e.await() ) ){

		this->HideUI() ;
	}else{
		if( m_ui->cbKeyType->currentIndex() == keyDialog::Key ){

			m_ui->lineEditKey->clear() ;
		}

		m_ui->lineEditKey->setFocus() ;

		this->enableAll() ;
	}
}

void keyDialog::encryptedFolderMount()
{
	auto ro = m_ui->checkBoxOpenReadOnly->isChecked() ;

	auto m = m_ui->lineEditMountPoint->text() ;

	if( utility::pathExists( m ) && !m_reUseMountPoint ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),tr( "Mount Point Path Already Taken." ) ) ;

		return this->enableAll() ;
	}

	if( !utility::pathExists( m_path ) ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),tr( "Encrypted Folder Appear To Not Be Present." ) ) ;

		return this->enableAll() ;
	}

        auto& e = cryfsTask::encryptedFolderMount( { m_path,m,m_key,m_options,m_configFile,ro,m_success } ) ;

	if( this->completed( e.await() ) ){

		m_success( m ) ;

		this->HideUI() ;
	}else{
		m_ui->cbKeyType->setCurrentIndex( keyDialog::Key ) ;

		m_ui->lineEditKey->clear() ;
		m_ui->lineEditKey->setFocus() ;

		this->enableAll() ;
	}
}

void keyDialog::openVolume()
{
	auto keyType = m_ui->cbKeyType->currentIndex() ;

	if( keyType == keyDialog::Key || keyType == keyDialog::keyKeyFile ){

		m_key = m_ui->lineEditKey->text() ;

	}else if( keyType == keyDialog::keyfile ){

		QFile f( m_ui->lineEditKey->text() ) ;

		f.open( QIODevice::ReadOnly ) ;

		m_key = f.readAll() ;

	}else if( keyType == keyDialog::plugin ){

		/*
		 * m_key is already set
		 */
	}

	if( m_create ){

		this->encryptedFolderCreate() ;
	}else{
		this->encryptedFolderMount() ;
	}
}

void keyDialog::cbActicated( int e )
{
	switch( e ){

		case keyDialog::Key        : return this->key() ;
		case keyDialog::keyfile    : return this->keyFile() ;
		case keyDialog::keyKeyFile : return this->keyAndKeyFile() ;
		case keyDialog::plugin     : return this->plugIn() ;
	}
}

void keyDialog::keyAndKeyFile()
{
	QString key ;

	if( utility::pluginKey( this,&key,"hmac" ) ){

		m_ui->cbKeyType->setCurrentIndex( 0 ) ;
	}else{
		this->key() ;

		m_ui->lineEditKey->setEnabled( false ) ;
		m_ui->lineEditKey->setText( key ) ;
	}
}

void keyDialog::plugIn()
{
	m_ui->pbkeyOption->setIcon( QIcon( ":/module.png" ) ) ;
	m_ui->lineEditKey->setEchoMode( QLineEdit::Normal ) ;
	m_ui->label->setText( tr( "Plugin name" ) ) ;
	m_ui->pbkeyOption->setEnabled( true ) ;
	m_ui->lineEditKey->setEnabled( false ) ;
	m_ui->lineEditKey->setText( INTERNAL_WALLET ) ;
}

void keyDialog::key()
{
	m_ui->pbkeyOption->setIcon( QIcon( ":/passphrase.png" ) ) ;
	m_ui->pbkeyOption->setEnabled( false ) ;
	m_ui->label->setText( tr( "Key" ) ) ;
	m_ui->lineEditKey->setEchoMode( QLineEdit::Password ) ;
	m_ui->lineEditKey->clear() ;
	m_ui->lineEditKey->setEnabled( true ) ;
}

void keyDialog::keyFile()
{
	m_ui->pbkeyOption->setIcon( QIcon( ":/keyfile.png" ) ) ;
	m_ui->lineEditKey->setEchoMode( QLineEdit::Normal ) ;
	m_ui->label->setText( tr( "Keyfile path" ) ) ;
	m_ui->pbkeyOption->setEnabled( true ) ;
	m_ui->lineEditKey->clear() ;
	m_ui->lineEditKey->setEnabled( true ) ;
}

void keyDialog::pbCancel()
{
	this->HideUI() ;
	m_cancel() ;
}

void keyDialog::ShowUI()
{
	this->show() ;
}

void keyDialog::HideUI()
{
	if( !m_working ){

		this->hide() ;
		this->deleteLater() ;
	}
}

keyDialog::~keyDialog()
{
	m_menu->deleteLater() ;
	delete m_ui ;
}
