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
#include "lxqt_wallet.h"

static QString _kwallet()
{
	return QObject::tr( "Kde Wallet" ) ;
}

static QString _internalWallet()
{
	return QObject::tr( "Internal Wallet" ) ;
}

static QString _gnomeWallet()
{
	return QObject::tr( "Gnome Wallet" ) ;
}

keyDialog::keyDialog( QWidget * parent,
		      QTableWidget * table,
		      secrets& s,
		      const volumeInfo& e,
		      std::function< void() > p,
		      std::function< void( const QString& ) > q,
		      const QString& exe ) :
	QDialog( parent ),
	m_ui( new Ui::keyDialog ),
	m_exe( exe ),
	m_secrets( s ),
	m_cancel( std::move( p ) ),
	m_success( std::move( q ) )
{
	m_ui->setupUi( this ) ;

	m_parentWidget = parent ;

	m_ui->checkBoxShareMountPoint->setVisible( false ) ;

	m_table = table ;
	m_path = e.volumeName() ;
	m_working = false ;

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

		this->windowSetTitle() ;
	}else{
		this->windowSetTitle( tr( "Unlocking \"%1\"" ).arg( m_path ) ) ;

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


	m_ui->pbOpenFolderPath->setIcon( QIcon( ":/folder.png" ) ) ;

	this->setFixedSize( this->size() ) ;
	this->setWindowFlags( Qt::Window | Qt::Dialog ) ;
	this->setFont( parent->font() ) ;

	m_ui->checkBoxOpenReadOnly->setChecked( utility::getOpenVolumeReadOnlyOption() ) ;

	m_ui->lineEditKey->setEchoMode( QLineEdit::Password ) ;

	connect( m_ui->pbCancel,SIGNAL( clicked() ),
		 this,SLOT( pbCancel() ) ) ;
	connect( m_ui->pbOpen,SIGNAL( clicked() ),
		 this,SLOT( pbOpen() ) ) ;
	connect( m_ui->pbkeyOption,SIGNAL( clicked() ),
		 this,SLOT( pbkeyOption() ) ) ;
	connect( m_ui->pbOpenFolderPath,SIGNAL( clicked() ),
		 this,SLOT( pbFolderPath() ) ) ;
	connect( m_ui->pbMountPoint,SIGNAL( clicked() ),
		 this,SLOT( pbMountPointPath() ) ) ;
	connect( m_ui->checkBoxOpenReadOnly,SIGNAL( stateChanged( int ) ),
		 this,SLOT( cbMountReadOnlyStateChanged( int ) ) ) ;
	connect( m_ui->cbKeyType,SIGNAL( currentIndexChanged( QString ) ),
		 this,SLOT( cbActicated( QString ) ) ) ;
	connect( m_ui->lineEditMountPoint,SIGNAL( textChanged( QString ) ),
		 this,SLOT( textChanged( QString ) ) ) ;

	m_ui->cbKeyType->addItem( tr( "Key" ) ) ;
	m_ui->cbKeyType->addItem( tr( "KeyFile" ) ) ;
	m_ui->cbKeyType->addItem( tr( "Key+KeyFile" ) ) ;

	m_ui->cbKeyType->addItem( _internalWallet() ) ;

	if( LXQt::Wallet::backEndIsSupported( LXQt::Wallet::BackEnd::libsecret ) ){

		m_ui->cbKeyType->addItem( _gnomeWallet() ) ;
	}

	if( LXQt::Wallet::backEndIsSupported( LXQt::Wallet::BackEnd::kwallet ) ){

		m_ui->cbKeyType->addItem( _kwallet() ) ;
	}

	m_ui->checkBoxShareMountPoint->setEnabled( false ) ;

	if( m_create ){

		if( m_keyStrength.canCheckQuality() ){

			connect( m_ui->lineEditKey,SIGNAL( textChanged( QString ) ),
				 this,SLOT( passWordTextChanged( QString ) ) ) ;
		}

		m_ui->lineEditMountPoint->setFocus() ;
	}else{
		m_ui->lineEditKey->setFocus() ;
	}

	this->installEventFilter( this ) ;

	connect( m_ui->pbOptions,SIGNAL( clicked() ),this,SLOT( pbOptions() ) ) ;
}

void keyDialog::windowSetTitle( const QString& s )
{
	if( s.isEmpty() ){

		auto f = tr( "Create A New \"%1\" Volume" ).arg( m_exe ) ;

		if( this->windowTitle() != f ){

			this->setWindowTitle( f ) ;
		}
	}else{
		this->setWindowTitle( s ) ;
	}
}

void keyDialog::pbOptions()
{
	options::instance( m_parentWidget,[ this ]( const QStringList& e ){

		m_options = e.at( 0 ) ;

		m_configFile = e.at( 1 ) ;

		if( m_ui->lineEditKey->text().isEmpty() ){

			m_ui->lineEditKey->setFocus() ;
		}else{
			m_ui->pbOpen->setFocus() ;
		}
	} ) ;
}

void keyDialog::passWordTextChanged( QString e )
{
	if( m_keyType == keyDialog::Key ){

		int r = m_keyStrength.quality( e ) ;

		if( r < 0 ){

			this->setWindowTitle( tr( "Passphrase Quality: 0%" ) ) ;
		}else{
			this->setWindowTitle( tr( "Passphrase Quality: %1%" ).arg( QString::number( r ) ) ) ;
		}

	}else if( m_keyType == keyDialog::keyKeyFile ){

		this->setWindowTitle( tr( "Passphrase Quality: 100%" ) ) ;
	}else{
		this->windowSetTitle() ;
	}
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

	auto index = m_ui->cbKeyType->currentIndex() ;

	m_ui->lineEditKey->setEnabled( index == keyDialog::Key ) ;

	auto enable = index == keyDialog::keyfile || index == keyDialog::keyKeyFile ;

	m_ui->pbkeyOption->setEnabled( enable ) ;

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
	this->KeyFile() ;
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

	if( m_ui->cbKeyType->currentIndex() > keyDialog::keyKeyFile ){

		utility::wallet w ;

		auto wallet = m_ui->lineEditKey->text() ;

		auto kde      = wallet == _kwallet() ;
		auto gnome    = wallet == _gnomeWallet() ;
		auto internal = wallet == _internalWallet() ;

		if( kde || gnome ){

			w = utility::getKey( m_path,m_secrets.walletBk( [ & ](){

				if( kde ){

					return LXQt::Wallet::BackEnd::kwallet ;
				}else{
					return LXQt::Wallet::BackEnd::libsecret ;
				}

			}() ).bk() ) ;

		}else if( internal ){

			using bk = LXQt::Wallet::BackEnd ;

			w = utility::getKey( m_path,m_secrets.walletBk( bk::internal ).bk() ) ;

			if( w.notConfigured ){

				DialogMsg msg( this ) ;
				msg.ShowUIOK( tr( "ERROR!" ),tr( "Internal Wallet Is Not Configured." ) ) ;
				return this->enableAll() ;
			}
		}else{
			return this->openVolume() ;
		}

		if( w.opened ){

			if( w.key.isEmpty() ){

				DialogMsg msg( this ) ;

				msg.ShowUIOK( tr( "ERROR" ),tr( "The Volume Does Not Appear To Have An Entry In The Wallet." ) ) ;

				this->enableAll() ;

				m_ui->lineEditKey->setEnabled( false ) ;
			}else{
				m_key = w.key ;
				this->openVolume() ;
			}
		}else{
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

	if( m_key.isEmpty() ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),tr( "Atleast One Required Field Is Empty." ) ) ;

		return this->enableAll() ;
	}

	auto& e = cryfsTask::encryptedFolderCreate( { path,m,m_key,m_options,m_configFile,
						      m_exe.toLower(),false,m_success } ) ;

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

	if( m_key.isEmpty() ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),tr( "Atleast One Required Field Is Empty." ) ) ;

		return this->enableAll() ;
	}

	auto& e = cryfsTask::encryptedFolderMount( { m_path,m,m_key,m_options,
						     m_configFile,m_exe,ro,m_success } ) ;

	if( this->completed( e.await() ) ){

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

	if( keyType == keyDialog::Key ){

		m_key = m_ui->lineEditKey->text() ;

	}if( keyType == keyDialog::keyKeyFile ){

		if( utility::pluginKey( m_secrets.parent(),&m_key,"hmac" ) ){

			return this->enableAll() ;
		}

	}else if( keyType == keyDialog::keyfile ){

		QFile f( m_ui->lineEditKey->text() ) ;

		f.open( QIODevice::ReadOnly ) ;

		m_key = f.readAll() ;

		if( m_key.contains( '\n' ) ){

			DialogMsg msg( this ) ;

			msg.ShowUIOK( tr( "WARNING" ),tr( "KeyFile Contents Will Be Trancated On The First Encountered NewLine Character." ) ) ;
		}

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

void keyDialog::cbActicated( QString e )
{
	e.remove( '&' ) ;

	if( e == tr( "Key" ) ){

		this->key() ;

	}else if( e == tr( "KeyFile" ) ){

		this->keyFile() ;

	}else if( e == tr( "Key+KeyFile" ) ){

		this->keyAndKeyFile() ;
	}else{
		this->plugIn() ;

		if( e == _kwallet() ){

			m_ui->lineEditKey->setText( _kwallet() ) ;

		}else if( e == _gnomeWallet() ){

			m_ui->lineEditKey->setText( _gnomeWallet() ) ;

		}else if( e == _internalWallet() ){

			m_ui->lineEditKey->setText( _internalWallet() ) ;
		}
	}
}

void keyDialog::keyAndKeyFile()
{
	m_keyType = keyDialog::keyKeyFile ;

	m_ui->pbkeyOption->setIcon( QIcon( ":/module.png" ) ) ;
	m_ui->pbkeyOption->setEnabled( false ) ;
	m_ui->lineEditKey->setEchoMode( QLineEdit::Normal ) ;
	m_ui->label->setText( tr( "Plugin name" ) ) ;
	m_ui->lineEditKey->setEnabled( false ) ;
	m_ui->lineEditKey->setText( tr( "Key+KeyFile" ) ) ;
}

void keyDialog::plugIn()
{
	m_keyType = keyDialog::plugin ;

	m_ui->pbkeyOption->setIcon( QIcon( ":/module.png" ) ) ;
	m_ui->pbkeyOption->setEnabled( false ) ;
	m_ui->lineEditKey->setEchoMode( QLineEdit::Normal ) ;
	m_ui->label->setText( tr( "Plugin name" ) ) ;
	m_ui->lineEditKey->setEnabled( false ) ;
	m_ui->lineEditKey->setText( _internalWallet() ) ;
}

void keyDialog::key()
{
	m_keyType = keyDialog::Key ;

	m_ui->pbkeyOption->setIcon( QIcon( ":/passphrase.png" ) ) ;
	m_ui->pbkeyOption->setEnabled( false ) ;
	m_ui->label->setText( tr( "Key" ) ) ;
	m_ui->lineEditKey->setEchoMode( QLineEdit::Password ) ;
	m_ui->lineEditKey->clear() ;
	m_ui->lineEditKey->setEnabled( true ) ;
}

void keyDialog::keyFile()
{
	m_keyType = keyDialog::keyfile ;

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
	delete m_ui ;
}
