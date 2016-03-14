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

keyDialog::keyDialog( QWidget * parent,QTableWidget * table,const volumeEntryProperties& e,std::function< void() > p,std::function< void( const QString& ) > q ) :
	QDialog( parent ),m_ui( new Ui::keyDialog ),m_cancel( std::move( p ) ),m_success( std::move( q ) )
{
	m_ui->setupUi( this ) ;

	m_ui->checkBoxShareMountPoint->setVisible( false ) ;

	m_table = table ;
	m_path = e.volumeName() ;
	m_working = false ;

	decltype( tr( "" ) ) msg ;

	m_create = e.volumeName().isEmpty() ;

	if( m_create ){

		m_ui->pbOpen->setText( tr( "&Create" ) ) ;

		m_ui->label_2->setText( tr( "Volume Name" ) ) ;

		m_ui->label_3->setVisible( true ) ;

		m_ui->checkBoxOpenReadOnly->setVisible( false ) ;

		m_ui->lineEditFolderPath->setVisible( true ) ;

		m_ui->pbOpenMountPoint->setVisible( true ) ;

		m_ui->lineEditFolderPath->setText( utility::homePath() + "/" ) ;

		m_ui->lineEditFolderPath->setEnabled( false ) ;

		m_ui->lineEditMountPoint->setFocus() ;

		msg = tr( "Create A New Cryfs Volume" ) ;
	}else{
		m_ui->lineEditMountPoint->setEnabled( false ) ;

		m_ui->label_3->setVisible( false ) ;

		m_ui->lineEditFolderPath->setVisible( false ) ;

		m_ui->checkBoxOpenReadOnly->setVisible( true ) ;

		m_ui->pbOpenMountPoint->setVisible( false ) ;

		m_ui->lineEditKey->setFocus() ;

		msg = tr( "Unlocking \"%1\"" ).arg( m_path ) ;
	}

	this->setWindowTitle( msg ) ;

	m_ui->lineEditMountPoint->setText( m_path ) ;
	m_ui->pbOpenMountPoint->setIcon( QIcon( ":/folder.png" ) ) ;

	m_menu = new QMenu( this ) ;

	connect( m_menu,SIGNAL( triggered( QAction * ) ),this,SLOT( pbPluginEntryClicked( QAction * ) ) ) ;

	this->setFixedSize( this->size() ) ;
	this->setWindowFlags( Qt::Window | Qt::Dialog ) ;
	this->setFont( parent->font() ) ;

	m_ui->checkBoxOpenReadOnly->setChecked( utility::getOpenVolumeReadOnlyOption( "cryfs-gui" ) ) ;

	m_ui->pbkeyOption->setEnabled( false ) ;

	m_ui->lineEditKey->setEchoMode( QLineEdit::Password ) ;

	connect( m_ui->pbOptions,SIGNAL( clicked() ),this,SLOT( pbOptions() ) ) ;
	connect( m_ui->pbCancel,SIGNAL( clicked() ),this,SLOT( pbCancel() ) ) ;
	connect( m_ui->pbOpen,SIGNAL( clicked() ),this,SLOT( pbOpen() ) ) ;
	connect( m_ui->pbkeyOption,SIGNAL( clicked() ),this,SLOT( pbkeyOption() ) ) ;
	connect( m_ui->pbOpenMountPoint,SIGNAL( clicked() ),this,SLOT( pbMountPointPath() ) ) ;
	connect( m_ui->checkBoxOpenReadOnly,SIGNAL( stateChanged( int ) ),this,SLOT( cbMountReadOnlyStateChanged( int ) ) ) ;
	connect( m_ui->cbKeyType,SIGNAL( currentIndexChanged( int ) ),this,SLOT( cbActicated( int ) ) ) ;
	connect( m_ui->lineEditMountPoint,SIGNAL( textChanged( QString ) ),this,SLOT( textChanged( QString ) ) ) ;

	const auto& m = e.mountPoint() ;

	if( m.isEmpty() || m == "Nil" ){

		m_point = utility::mountPathPostFix( m_path.split( "/" ).last() ) ;
	}else{
		m_point = utility::mountPathPostFix( m ) ;
	}

	m_ui->lineEditMountPoint->setText( m_point ) ;

	auto ac = new QAction( this ) ;
	QKeySequence s( Qt::CTRL + Qt::Key_F ) ;
	ac->setShortcut( s ) ;
	connect( ac,SIGNAL( triggered() ),this,SLOT( showOffSetWindowOption() ) ) ;
	this->addAction( ac ) ;

	m_menu_1 = new QMenu( this ) ;

	m_menu_1->setFont( this->font() ) ;

	auto _add_action = [ & ]( const QString& e ){

		ac = m_menu_1->addAction( e ) ;
		ac ->setEnabled( false ) ;
	} ;

	_add_action( tr( "Set File System Options" ) ) ;

	m_ui->cbKeyType->addItem( tr( "Key" ) ) ;
	m_ui->cbKeyType->addItem( tr( "KeyFile" ) ) ;
	m_ui->cbKeyType->addItem( tr( "Key+KeyFile" ) ) ;
	m_ui->cbKeyType->addItem( tr( "Plugin" ) ) ;

	m_ui->checkBoxShareMountPoint->setEnabled( false ) ;

	connect( m_menu_1,SIGNAL( triggered( QAction * ) ),this,SLOT( doAction( QAction * ) ) ) ;

	if( m_create ){

		m_ui->lineEditMountPoint->setFocus() ;
	}else{
		m_ui->lineEditKey->setFocus() ;
	}

	this->installEventFilter( this ) ;
}

bool keyDialog::eventFilter( QObject * watched,QEvent * event )
{
	return utility::eventFilter( this,watched,event,[ this ](){ this->pbCancel() ; } ) ;
}

void keyDialog::pbOptions()
{
	m_menu_1->exec( QCursor::pos() ) ;
}

void keyDialog::showOffSetWindowOption()
{
}

void keyDialog::showFileSystemOptionWindow()
{
}

void keyDialog::doAction( QAction * ac )
{
	Q_UNUSED( ac ) ;
}

void keyDialog::cbMountReadOnlyStateChanged( int state )
{
	Q_UNUSED( state ) ;

	m_ui->checkBoxOpenReadOnly->setEnabled( false ) ;
	m_ui->checkBoxOpenReadOnly->setChecked( utility::setOpenVolumeReadOnly( this,state == Qt::Checked,QString( "cryfs-gui" ) ) ) ;
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
	auto msg = tr( "Select A Folder To Create A Mount Point In" ) ;
	auto e = QFileDialog::getExistingDirectory( this,msg,utility::homePath(),QFileDialog::ShowDirsOnly ) ;

	if( !e.isEmpty() ){

		e = e + "/" + m_ui->lineEditFolderPath->text().split( '/' ).last() ;

		m_ui->lineEditFolderPath->setText( e ) ;
	}
}

void keyDialog::enableAll()
{
	m_ui->pbOptions->setEnabled( true ) ;
	m_ui->label_2->setEnabled( true ) ;
	m_ui->lineEditMountPoint->setEnabled( true ) ;
	m_ui->pbOpenMountPoint->setEnabled( true ) ;
	m_ui->pbCancel->setEnabled( true ) ;
	m_ui->pbOpen->setEnabled( true ) ;
	m_ui->label->setEnabled( true ) ;
	m_ui->cbKeyType->setEnabled( true ) ;

	m_ui->lineEditKey->setEnabled( m_ui->cbKeyType->currentIndex() == keyDialog::Key ) ;

	m_ui->pbkeyOption->setEnabled( true ) ;
	m_ui->checkBoxOpenReadOnly->setEnabled( true ) ;

	m_ui->checkBoxShareMountPoint->setEnabled( false ) ;
	//m_ui->lineEditFolderPath->setEnabled( true ) ;
	m_ui->label_3->setEnabled( true ) ;
}

void keyDialog::disableAll()
{
	m_ui->cbKeyType->setEnabled( false ) ;
	m_ui->pbOptions->setEnabled( false ) ;
	m_ui->pbkeyOption->setEnabled( false ) ;
	m_ui->label_2->setEnabled( false ) ;
	m_ui->label_3->setEnabled( false ) ;
	m_ui->lineEditMountPoint->setEnabled( false ) ;
	m_ui->pbOpenMountPoint->setEnabled( false ) ;
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

		auto msg = tr( "Select A File To Be Used As A Keyfile" ) ;
		auto Z = QFileDialog::getOpenFileName( this,msg,utility::homePath() ) ;

		if( !Z.isEmpty() ){

			m_ui->lineEditKey->setText( Z ) ;
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
	utility::createPlugInMenu( m_menu,tr( INTERNAL_WALLET ),
				   tr( GNOME_WALLET ),tr( KWALLET ),true ) ;

	m_menu->setFont( this->font() ) ;

	m_menu->addSeparator() ;

	m_menu->addAction( tr( "Cancel" ) ) ;

	m_menu->exec( QCursor::pos() ) ;
}

void keyDialog::pbPluginEntryClicked( QAction * e )
{
	auto r = e->text() ;

	r.remove( "&" ) ;

	if( r != tr( "Cancel" ) ){

		m_ui->lineEditKey->setText( r ) ;
	}
}

void keyDialog::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->pbCancel() ;
}

void keyDialog::pbOpen()
{
	this->disableAll() ;

	if( m_ui->cbKeyType->currentIndex() == keyDialog::plugin ){

		utility::wallet w ;

		auto wallet = m_ui->lineEditKey->text() ;

		if( wallet == tr( KWALLET ) ){

			w = utility::getKeyFromWallet( LxQt::Wallet::kwalletBackEnd,m_path ) ;

		}else if( wallet == tr( INTERNAL_WALLET ) ){

			w = utility::getKeyFromWallet( LxQt::Wallet::internalBackEnd,m_path,_internalPassWord ) ;

			if( w.notConfigured ){

				DialogMsg msg( this ) ;
				msg.ShowUIOK( tr( "ERROR!" ),tr( "Internal wallet is not configured" ) ) ;
				return this->enableAll() ;
			}else{
				_internalPassWord = w.password ;
			}

		}else if( wallet == tr( GNOME_WALLET ) ){

			w = utility::getKeyFromWallet( LxQt::Wallet::secretServiceBackEnd,m_path ) ;
		}else{
			return this->openVolume() ;
		}

		if( w.opened ){

			if( w.key.isEmpty() ){

				DialogMsg msg( this ) ;

				msg.ShowUIOK( tr( "ERROR" ),tr( "The volume does not appear to have an entry in the wallet" ) ) ;

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

bool keyDialog::completed( cryfsTask::encryptedVolume::status s )
{
	DialogMsg msg( this ) ;

	switch( s ){

	using ev = cryfsTask::encryptedVolume ;

	case ev::status::success :

		return true ;

	case ev::status::cryfs :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to unlock a cryfs volume.\nWrong password entered" ) ) ;
		break;

	case ev::status::encfs :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to unlock an encfs volume.\nWrong password entered" ) ) ;
		break;

	case ev::status::cryfsNotFound :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to unlock a cryfs volume.\ncryfs executable could not be found" ) ) ;
		break;

	case ev::status::encfsNotFound :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to unlock an encfs volume.\nencfs executable could not be found" ) ) ;
		break;

	case ev::status::failedToCreateMountPoint :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to create mount point" ) ) ;
		break;

	case ev::status::unknown :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to unlock the volume.\nNot supported volume encountered" ) ) ;
		break;

	case ev::status::backendFail :

		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to complete the task.\nBackend not responding" ) ) ;
		break;
	default:
		msg.ShowUIOK( tr( "ERROR" ),tr( "Failed to complete the task.\nAn unknown error has occured" ) ) ;
		break;
	}

	return false ;
}

void keyDialog::encryptedFolderCreate()
{
	auto path = m_ui->lineEditFolderPath->text() ;

	if( this->completed( cryfsTask::encryptedFolderCreate( path,path.split( '/' ).last(),m_key,m_success ).await().state ) ){

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
	auto m = utility::mountPath( m_ui->lineEditMountPoint->text() ) ;

	auto ro = m_ui->checkBoxOpenReadOnly->isChecked() ;

	if( this->completed( cryfsTask::encryptedFolderMount( m_path,m,m_key,ro ).await().state ) ){

		m_success( m ) ;

		this->HideUI() ;
	}else{
		if( m_ui->cbKeyType->currentIndex() == keyDialog::Key ){

			m_ui->lineEditKey->clear() ;
		}

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
