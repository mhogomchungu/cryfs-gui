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

#include "cryfs-gui.h"
#include "ui_cryfs-gui.h"
#include <QDebug>

#include <QMainWindow>

#include <QTableWidgetItem>
#include <QDir>
#include <QIcon>
#include <QAction>
#include <QKeySequence>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QFont>
#include <QCursor>
#include <QAction>
#include <QFileDialog>
#include <QUrl>
#include <QTranslator>
#include <QMimeData>
#include <QFile>

#include <utility>
#include <initializer_list>

#include <unistd.h>
#include <sys/vfs.h>

#include "keydialog.h"
#include "dialogmsg.h"
#include "tablewidget.h"
#include "oneinstance.h"
#include "monitor_mountinfo.h"
#include "utility.h"
#include "cryfstask.h"
#include "checkforupdates.h"
#include "favorites.h"
#include "walletconfig.h"

#include <memory>

cryfsGUI::cryfsGUI( QWidget * parent ) :
	QWidget( parent ),
	m_mountInfo( monitor_mountinfo::instance( this,true,[](){ QCoreApplication::quit() ; } ) )
{
}

void cryfsGUI::setUpApp( const QString& volume )
{
	this->setLocalizationLanguage( true ) ;

	m_ui = new Ui::cryfsGUI ;
	m_ui->setupUi( this ) ;

	m_ui->pbcreate->setMinimumHeight( 31 ) ;
	m_ui->pbunlockcryptfs->setMinimumHeight( 31 ) ;
	m_ui->pbmenu->setMinimumHeight( 31 ) ;
	m_ui->pbupdate->setMinimumHeight( 31 ) ;

	auto f = utility::getWindowDimensions() ;

	auto e = f.data() ;

	this->window()->setGeometry( *( e + 0 ),*( e + 1 ),*( e + 2 ),*( e + 3 ) ) ;

	auto table = m_ui->tableWidget ;

	table->setColumnWidth( 0,*( e + 4 ) ) ;
	table->setColumnWidth( 1,*( e + 5 ) ) ;
	table->setColumnWidth( 2,*( e + 6 ) ) ;
	table->setColumnWidth( 3,*( e + 7 ) ) ;

#if QT_VERSION < QT_VERSION_CHECK( 5,0,0 )
	m_ui->tableWidget->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents ) ;
#else
	m_ui->tableWidget->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents ) ;
#endif
	m_ui->tableWidget->verticalHeader()->setMinimumSectionSize( 30 ) ;

	m_ui->tableWidget->setMouseTracking( true ) ;

	connect( m_ui->tableWidget,SIGNAL( itemEntered( QTableWidgetItem * ) ),
		 this,SLOT( itemEntered( QTableWidgetItem * ) ) ) ;

	connect( m_ui->tableWidget,SIGNAL( currentItemChanged( QTableWidgetItem *,QTableWidgetItem * ) ),
		 this,SLOT( slotCurrentItemChanged( QTableWidgetItem *,QTableWidgetItem * ) ) ) ;

	connect( m_ui->pbupdate,SIGNAL( clicked()),
		 this,SLOT( pbUpdate() ) ) ;

	connect( &m_mountInfo,SIGNAL( gotEvent() ),this,SLOT( pbUpdate() ) ) ;

	connect( m_ui->tableWidget,SIGNAL( itemClicked( QTableWidgetItem * ) ),
		 this,SLOT( itemClicked( QTableWidgetItem * ) ) ) ;

	connect( m_ui->pbunlockcryptfs,SIGNAL( clicked() ),
		 this,SLOT( unlockCryptFs() ) ) ;

	connect( m_ui->pbcreate,SIGNAL( clicked() ),this,SLOT( pbCreate() ) ) ;

	m_autoOpenFolderOnMount = this->autoOpenFolderOnMount() ;

	this->setUpShortCuts() ;

	this->setUpFont() ;

	const auto& icon = utility::getIcon() ;

	this->setAcceptDrops( true ) ;
	this->setWindowIcon( icon ) ;

	this->setUpAppMenu() ;

	this->setLocalizationLanguage( false ) ;

	m_trayIcon.setParent( this ) ;
	m_trayIcon.setIcon( icon ) ;

	m_trayIcon.show() ;

	auto dirPath = utility::homeConfigPath() ;
	QDir dir( dirPath ) ;

	if( !dir.exists() ){

		dir.mkpath( dirPath ) ;
	}

	this->disableAll() ;

	this->startAutoMonitor() ;

	this->updateVolumeList( cryfsTask::updateVolumeList().await() ) ;

	if( volume.isEmpty() ) {

		this->enableAll() ;
	}else{
		this->showMoungDialog( volume ) ;
	}

	this->autoUpdateCheck() ;
}

void cryfsGUI::setUpAppMenu()
{
	auto m = new QMenu( this ) ;

	m->setFont( this->font() ) ;

	auto _addAction = [ this ]( bool p,bool q,const QString& e,const char * z,const char * s ){

		auto ac = new QAction( e,this ) ;

		m_actionPair.append( { ac,z } ) ;

		if( p ){

			ac->setCheckable( p ) ;
			ac->setChecked( q ) ;

			if( s ){

				connect( ac,SIGNAL( toggled( bool ) ),this,s ) ;
			}
		}else{
			if( s ){

				connect( ac,SIGNAL( triggered() ),this,s ) ;
			}
		}

		return ac ;
	} ;

	auto _addMenu = [ m,this ]( const QString& r,const char * z,const char * t,const char * s ){

		auto e = m->addMenu( r ) ;

		m_menuPair.append( { m,z } ) ;

		e->setFont( this->font() ) ;

		if( t ){

			connect( e,SIGNAL( triggered( QAction * ) ),this,t ) ;
		}

		if( s ){

			connect( e,SIGNAL( aboutToShow() ),this,s ) ;
		}

		return e ;
	} ;

	m->addAction( _addAction( true,m_autoOpenFolderOnMount,tr( "Auto Open Mount Point" ),
				  "Auto Open Mount Point",SLOT( autoOpenFolderOnMount( bool ) ) ) ) ;

	m->addAction( _addAction( true,utility::reUseMountPoint(),tr( "Reuse Mount Point" ),
				  "Reuse Mount Point",SLOT( reuseMountPoint( bool ) ) ) ) ;

	m->addAction( _addAction( true,checkForUpdates::autoCheck(),tr( "Autocheck For Updates" ),
				  "Autocheck For Updates",SLOT( autoCheckUpdates( bool ) ) ) ) ;

	m->addAction( _addAction( false,false,tr( "Unmount All" ),"Unmount All",SLOT( unMountAll() ) ) ) ;

	m_change_password_action = [ &_addMenu,&_addAction ](){

		auto m = _addMenu( tr( "Internal Wallet" ),"Internal Wallet",nullptr,nullptr ) ;

		auto ac = _addAction( false,false,tr( "Change Password" ),"Change Password",
				       SLOT( changeInternalWalletPassWord() ) ) ;

		m->addAction( ac ) ;

		return ac ;
	}() ;

	m_key_manager_menu = [ &_addMenu,&_addAction ](){

		auto m = _addMenu( tr( "Key Storage" ),"Key Storage",
				   SLOT( keyManagerClicked( QAction * ) ),
				   SLOT( aboutToShowMenu() ) ) ;

		auto _addOption = [ & ]( const QString& e,const char * z,LxQt::Wallet::walletBackEnd s ){

			auto ac = _addAction( false,false,e,z,nullptr ) ;

			ac->setEnabled( LxQt::Wallet::backEndIsSupported( s ) ) ;

			m->addAction( ac ) ;
		} ;

		_addOption( tr( "Internal Wallet" ),"Internal Wallet",LxQt::Wallet::internalBackEnd ) ;
		_addOption( tr( "KDE Wallet" ),"KDE Wallet",LxQt::Wallet::kwalletBackEnd ) ;
		_addOption( tr( "Gnome Wallet" ),"Gnome Wallet",LxQt::Wallet::secretServiceBackEnd ) ;

		return m ;
	}() ;

	m_favorite_menu = _addMenu( tr( "Favorites" ),"Favorites",
				    SLOT( favoriteClicked( QAction * ) ),
				    SLOT( showFavorites() ) ) ;

	m_language_menu = _addMenu( tr( "Select Language" ),"Select Language",
				    SLOT( languageMenu( QAction * ) ),nullptr ) ;

	m->addAction( _addAction( false,false,tr( "Check For Updates" ),"Check For Updates",
				  SLOT( updateCheck() ) ) ) ;

	m->addAction( _addAction( false,false,tr( "About" ),"About",SLOT( licenseInfo() ) ) ) ;

	auto ac = _addAction( false,false,tr( "Quit" ),"Quit",SLOT( closeApplication() ) ) ;

	m->addAction( ac ) ;

	m_ui->pbmenu->setMenu( m ) ;

	m_trayIcon.setContextMenu( [ this,ac ](){

		auto m = new QMenu( this ) ;

		m->addAction( ac ) ;

		return m ;
	}() ) ;

	connect( &m_trayIcon,SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
		 this,SLOT( slotTrayClicked( QSystemTrayIcon::ActivationReason ) ) ) ;
}

void cryfsGUI::startAutoMonitor()
{
	m_mountInfo.start() ;
}

/*
 * This should be the only function that closes the application
 */
void cryfsGUI::closeApplication()
{
	m_mountInfo.stop()() ;
}

void cryfsGUI::aboutToShowMenu()
{
	auto a = utility::walletName() ;
	auto b = utility::applicationName() ;
	auto c = LxQt::Wallet::walletExists( LxQt::Wallet::internalBackEnd,a,b ) ;
	m_change_password_action->setEnabled( c ) ;
}

void cryfsGUI::changeInternalWalletPassWord()
{
	changeWalletPassWord::instance( this ) ;
}

void cryfsGUI::keyManagerClicked( QAction * ac )
{
	auto e = ac->text() ;
	e.remove( "&" ) ;

	if( e == tr( "Internal Wallet" ) ){

		walletconfig::instance( this ).ShowUI( LxQt::Wallet::internalBackEnd ) ;

	}else if( e == tr( "KDE Wallet" ) ){

		walletconfig::instance( this ).ShowUI( LxQt::Wallet::kwalletBackEnd ) ;

	}else if( e == tr( "Gnome Wallet" ) ){

		walletconfig::instance( this ).ShowUI( LxQt::Wallet::secretServiceBackEnd ) ;
	}
}

void cryfsGUI::licenseInfo()
{
	utility::licenseInfo( this ) ;
}

void cryfsGUI::updateCheck()
{
	checkForUpdates::instance( this,false ) ;
}

void cryfsGUI::autoUpdateCheck()
{
	checkForUpdates::instance( this,true ) ;
}

void cryfsGUI::autoCheckUpdates( bool e )
{
	checkForUpdates::autoCheck( e ) ;
}

QString cryfsGUI::resolveFavoriteMountPoint( const QString& e )
{
	for( const auto& it : utility::readFavorites() ){

		if( it.startsWith( e + '\t' ) ){

			auto l = it.split( '\t' ) ;

			if( l.size() > 1 ){

				return l.at( 1 ) ;
			}else{
				return QString() ;
			}
		}
	}

	return QString() ;
}

void cryfsGUI::favoriteClicked( QAction * ac )
{
	auto e = ac->text() ;
	e.remove( "&" ) ;

	if( e == tr( "Manage Favorites" ) ){

		favorites::instance( this ) ;

	}else if( e == tr( "Mount All" ) ){

		for( const auto& it : utility::readFavorites() ){

			auto e = utility::split( it,'\t' ) ;

			if( e.size() > 1 ){

				this->showMoungDialog( e.at( 0 ),e.at( 1 ) ) ;
			}
		}

	}else{
		this->showMoungDialog( e,this->resolveFavoriteMountPoint( e ) ) ;
	}
}

void cryfsGUI::showFavorites()
{
	utility::readFavorites( m_favorite_menu,true ) ;
}

void cryfsGUI::setLocalizationLanguage( bool translate )
{
	utility::setLocalizationLanguage( translate,m_language_menu ) ;
}

void cryfsGUI::languageMenu( QAction * ac )
{
	utility::languageMenu( this,m_language_menu,ac ) ;

	m_ui->retranslateUi( this ) ;

	for( auto& it : m_actionPair ){

		it.first->setText( tr( it.second ) ) ;
	}

	for( auto& it : m_menuPair ){

		it.first->setTitle( tr( it.second ) ) ;
	}
}

static QString _autoOpenFolderConfigPath()
{
	return utility::homeConfigPath( "cryfs-gui.NoAutoOpenFolder" ) ;
}

void cryfsGUI::autoOpenFolderOnMount( bool e )
{
	m_autoOpenFolderOnMount = e ;

	if( m_autoOpenFolderOnMount ){

		QFile::remove( _autoOpenFolderConfigPath() ) ;
	}else{
		QFile f( _autoOpenFolderConfigPath() ) ;
		f.open( QIODevice::WriteOnly ) ;
	}
}

void cryfsGUI::reuseMountPoint( bool e )
{
	utility::reUseMountPoint( e ) ;
}

bool cryfsGUI::autoOpenFolderOnMount( void )
{
	return !QFile::exists( _autoOpenFolderConfigPath() ) ;
}

void cryfsGUI::itemEntered( QTableWidgetItem * item )
{
	Q_UNUSED( item ) ;
}

void cryfsGUI::startGUI()
{
	if( !m_startHidden ){

		this->raiseWindow() ;
	}
}

void cryfsGUI::raiseWindow()
{
	this->setVisible( true ) ;
	this->raise() ;
	this->show() ;
	this->setWindowState( Qt::WindowActive ) ;
}

void cryfsGUI::raiseWindow( QString volume )
{
	this->setVisible( true ) ;
	this->raise() ;
	this->show() ;
	this->setWindowState( Qt::WindowActive ) ;
	this->showMoungDialog( volume ) ;
}

void cryfsGUI::Show()
{
	auto l = QCoreApplication::arguments() ;

	m_startHidden  = l.contains( "-e" ) ;
	m_folderOpener = utility::cmdArgumentValue( l,"-m","xdg-open" ) ;
	m_env          = utility::cmdArgumentValue( l,"-z","" ) ;

	auto volume = utility::cmdArgumentValue( l,"-d" ) ;

	auto b = utility::cmdArgumentValue( l,"-b","" ) ;
	auto k = utility::cmdArgumentValue( l,"-k","rw" ) ;
	auto z = utility::cmdArgumentValue( l,"-z","" ) ;
	auto i = utility::cmdArgumentValue( l,"-i","" ) ;
        auto c = utility::cmdArgumentValue( l,"-c","" ) ;

	if( b.isEmpty() ){

		oneinstance::instance( this,"cryfs-gui.socket","startGUI",
				       volume,[ this,volume ]( QObject * instance ){

			connect( instance,SIGNAL( raise() ),this,
				 SLOT( raiseWindow() ) ) ;
			connect( instance,SIGNAL( raiseWithDevice( QString ) ),
				 this,SLOT( raiseWindow( QString ) ) ) ;

			this->setUpApp( volume ) ;
		} ) ;
	}else{
		QMetaObject::invokeMethod( this,"unlockVolume",Qt::QueuedConnection,
					   Q_ARG( QString,volume ),Q_ARG( QString,z ),
					   Q_ARG( QString,b ),Q_ARG( QString,i ),
                                           Q_ARG( QString,c ),Q_ARG( bool,k == "ro" ) ) ;
	}
}

void cryfsGUI::unlockVolume( const QString& volume,const QString& mountPath,
                             const QString& backEnd,const QString& mOpt,const QString& cPath,bool mode )
{
	if( volume.isEmpty() ){

		qDebug() << tr( "ERROR: Volume Path Not Given." ) ;
		QCoreApplication::exit( 1 ) ;
	}else{
		auto w = [ & ](){

			namespace wxt = LxQt::Wallet ;

			auto _supported = [ & ]( wxt::walletBackEnd e,const char * s ){

				return backEnd == s && wxt::backEndIsSupported( e ) ;
			} ;

			if( _supported( wxt::internalBackEnd,"internal" ) ){

				return utility::getKeyFromWallet( wxt::internalBackEnd,volume ) ;

			}else if( _supported( wxt::secretServiceBackEnd,"gnomewallet" ) ){

				return utility::getKeyFromWallet( wxt::secretServiceBackEnd,volume ) ;

			}else if( _supported( wxt::kwalletBackEnd,"kwallet" ) ){

				return utility::getKeyFromWallet( wxt::kwalletBackEnd,volume ) ;
			}else{
				return utility::wallet{ false,true,"","" } ;
			}
		}() ;

		if( w.opened ){

			if( w.key.isEmpty() ){

				qDebug() << tr( "ERROR: Key Not Found In The Backend." ) ;
				QCoreApplication::exit( 1 ) ;
			}else{
				auto m = [ & ]()->QString{

					if( mountPath.isEmpty() ){

						auto e = utility::mountPathPostFix( volume.split( "/" ).last() ) ;

						return utility::mountPath( e ) ;
					}else{
						return mountPath ;
					}
				}() ;

				auto o = []( const QString& e ){ Q_UNUSED( e ) ; } ;

                                auto e = cryfsTask::encryptedFolderMount( { volume,m,w.key,mOpt,cPath,mode,o } ).await() ;

				if( e == cryfsTask::status::success ){

					QCoreApplication::exit( 0 ) ;
				}else{
					qDebug() << tr( "ERROR: Failed To Unlock Requested Volume." ) ;
					QCoreApplication::exit( 1 ) ;
				}
			}
		}else{
			qDebug() << tr( "ERROR: Failed To Unlock Requested Backend." ) ;
			QCoreApplication::exit( 1 ) ;
		}
	}
}

void cryfsGUI::properties()
{
	auto mountPath = [ this ](){

		auto table = m_ui->tableWidget ;

		auto row = table->currentRow() ;

		if( row >= 0 ){

			return table->item( row,1 )->text() ;
		}else{
			return QString() ;
		}
	}() ;

	if( !mountPath.isEmpty() ){

		struct statfs vfs ;

		DialogMsg msg( this ) ;

		if( statfs( mountPath.toLatin1().constData(),&vfs ) == 0 ){

			msg.ShowUIOK( tr( "INFORMATION" ),tr( "Used Space: %1" ).arg( [ & ]{

				auto s = vfs.f_bsize * ( vfs.f_blocks - vfs.f_bavail ) ;

				auto _convert = [ & ]( const char * p,double q ){

					auto e = QString::number( double( s ) / q,'f',2 ) ;

					return QString( "%1 %2" ).arg( e,p ) ;
				} ;

				switch( QString::number( s ).size() ){

					case 0 :
					case 1 : case 2 : case 3 :

						return QString( "%1 B" ).arg( QString::number( s ) ) ;

					case 4 : case 5 : case 6 :

						return _convert( "KB",1024 ) ;

					case 7 : case 8 : case 9 :

						return _convert( "MB",1048576 ) ;

					case 10: case 11 : case 12 :

						return _convert( "GB",1073741824 ) ;

					default:
						return _convert( "TB",1024.0 * 1073741824 ) ;
				}
			}() ) ) ;
		}else{
			msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Read Volume Properties" ) ) ;
		}
	}
}

void cryfsGUI::showContextMenu( QTableWidgetItem * item,bool itemClicked )
{
	QMenu m ;

	m.setFont( this->font() ) ;

	auto _addAction = [ & ]( const QString& txt,const char * slot,bool enable ){

		auto ac = m.addAction( txt ) ;
		ac->setEnabled( enable ) ;

		connect( ac,SIGNAL( triggered() ),this,slot ) ;
	} ;

	_addAction( tr( "Unmount" ),SLOT( pbUmount() ),true ) ;

	_addAction( tr( "Open Folder" ),SLOT( slotOpenFolder() ),true ) ;

	_addAction( tr( "Properties" ),SLOT( properties() ),[ this ](){

		auto table = m_ui->tableWidget ;

		auto row = table->currentRow() ;

		if( row >= 0 ){

			return table->item( row,2 )->text() == "cryfs" ;
		}else{
			return false ;
		}
	}() ) ;

	m.addSeparator() ;

	m.addAction( tr( "Close Menu" ) ) ;

	if( itemClicked ){

		m.exec( QCursor::pos() ) ;
	}else{
		auto p = this->pos() ;

		auto x = p.x() + 100 + m_ui->tableWidget->columnWidth( 0 ) ;
		auto y = p.y() + 50 + m_ui->tableWidget->rowHeight( 0 ) * item->row() ;

		p.setX( x ) ;
		p.setY( y ) ;
		m.exec( p ) ;
	}
}

void cryfsGUI::itemClicked( QTableWidgetItem * item )
{
	this->showContextMenu( item,true ) ;
}

void cryfsGUI::defaultButton()
{
	auto table = m_ui->tableWidget ;

	if( table->rowCount() > 0 ){

		this->showContextMenu( table->currentItem(),false ) ;
	}
}

void cryfsGUI::slotOpenSharedFolder()
{
	this->openMountPoint( m_sharedFolderPath ) ;
}

void cryfsGUI::slotOpenFolder()
{
	auto table = m_ui->tableWidget ;

	if( table->rowCount() > 0 ){

		auto item = table->currentItem() ;
		auto path = table->item( item->row(),1 )->text() ;

		this->openMountPoint( path ) ;
	}
}

void cryfsGUI::openMountPoint( const QString& m_point )
{
	auto x = tr( "WARNING" ) ;
	auto y = tr( "Could Not Open Mount Point Because \"%1\" Tool Does Not Appear To Be Working Correctly." ).arg( m_folderOpener ) ;

	utility::openPath( m_point,m_folderOpener,m_env,this,x,y ) ;
}

void cryfsGUI::openMountPointPath( QString m )
{
	if( m_autoOpenFolderOnMount ){

		this->openMountPoint( m ) ;
	}
}

void cryfsGUI::setUpShortCuts()
{
	auto _addAction = [ this ]( std::initializer_list<QKeySequence> s,const char * slot ){

		auto ac = new QAction( this ) ;

		ac->setShortcuts( s ) ;

		connect( ac,SIGNAL( triggered() ),this,slot ) ;

		return ac ;
	} ;

	this->addAction( _addAction( { Qt::Key_Enter,Qt::Key_Return },SLOT( defaultButton() ) ) ) ;

	this->addAction( _addAction( { Qt::Key_M },SLOT( pbCreate() ) ) ) ;

	this->addAction( _addAction( { Qt::Key_U },SLOT( pbUmount() ) ) ) ;

	this->addAction( _addAction( { Qt::Key_R },SLOT( pbUpdate() ) ) ) ;

	this->addAction( _addAction( { Qt::Key_C },SLOT( closeApplication() ) ) ) ;
}

void cryfsGUI::setUpFont()
{
	this->setFont( utility::getFont( this ) ) ;
}

void cryfsGUI::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->hide() ;
}

void cryfsGUI::slotTrayClicked( QSystemTrayIcon::ActivationReason e )
{
	if( e == QSystemTrayIcon::Trigger ){

		if( this->isVisible() ){

			this->hide() ;
		}else{
			this->show() ;
		}
	}
}

void cryfsGUI::dragEnterEvent( QDragEnterEvent * e )
{
	e->accept() ;
}

void cryfsGUI::dropEvent( QDropEvent * e )
{
	for( const auto& it : e->mimeData()->urls() ){

		this->showMoungDialog( it.path() ) ;
	}
}

void cryfsGUI::mount( const volumeInfo& entry )
{
	this->disableAll() ;

	keyDialog::instance( this,m_ui->tableWidget,entry,[ this ](){

		this->enableAll() ;

	},[ this ]( const QString& e ){

		this->openMountPointPath( e ) ;

	} ).ShowUI() ;
}

void cryfsGUI::pbCreate()
{
	this->mount( volumeInfo() ) ;
}

void cryfsGUI::slotMount()
{
	auto table = m_ui->tableWidget ;

	if( table->rowCount() > 0 ){

		auto row = table->currentRow() ;

		this->mount( tablewidget::rowEntries( table,row ) ) ;
	}
}

void cryfsGUI::showMoungDialog( const volumeInfo& v )
{
	if( v.isEmpty() ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),
			      tr( "Permission To Access The Volume Was Denied\nOr\nThe Volume Is Not Supported" ) ) ;

		this->enableAll() ;
	}else{
		this->mount( v ) ;
	}
}

void cryfsGUI::showMoungDialog( const QString& volume,const QString& m_point )
{
	if( !volume.isEmpty() ){

		this->mount( { volume,m_point } ) ;
	}
}

void cryfsGUI::unlockCryptFs()
{
	this->disableAll() ;

	auto e = tr( "Select An Encfs/Cryfs Volume Directory" ) ;
	auto path = QFileDialog::getExistingDirectory( this,e,utility::homePath(),QFileDialog::ShowDirsOnly ) ;

	if( path.isEmpty() ){

		this->enableAll() ;
	}else{
		this->showMoungDialog( path ) ;
	}
}

QFont cryfsGUI::getSystemVolumeFont()
{
	auto f = this->font() ;
	f.setItalic( !f.italic() ) ;
	f.setBold( !f.bold() ) ;
	return f ;
}

void cryfsGUI::addEntryToTable( const QStringList& l )
{
	tablewidget::addRow( m_ui->tableWidget,l ) ;
}

void cryfsGUI::addEntryToTable( const volumeInfo& e )
{
	this->addEntryToTable( e.entryList() ) ;
}

void cryfsGUI::removeEntryFromTable( QString volume )
{
	auto table = m_ui->tableWidget ;

	auto r = tablewidget::columnHasEntry( table,volume ) ;

	if( r != -1 ){

		tablewidget::deleteRow( table,r ) ;
		this->enableAll() ;
	}else{
		this->pbUpdate() ;
	}
}

void cryfsGUI::updateList( const volumeInfo& entry )
{
	if( entry.isNotEmpty() ){

		auto table = m_ui->tableWidget ;

		auto row = tablewidget::columnHasEntry( table,entry.volumeName() ) ;

		if( row == -1 ){

			row = tablewidget::addRow( table ) ;
		}

		tablewidget::updateRow( table,entry.entryList(),row,this->font() ) ;

		tablewidget::selectRow( table,row ) ;
	}
}

void cryfsGUI::pbUmount()
{
	auto table = m_ui->tableWidget ;

	if( table->rowCount() > 0 ){

		this->disableAll() ;

		auto row = table->currentRow() ;

		auto type = table->item( row,2 )->text() ;

		auto m = table->item( row,1 )->text() ;

		if( !cryfsTask::encryptedFolderUnMount( m ).await() ){

			DialogMsg m( this ) ;
			m.ShowUIOK( tr( "ERROR" ),tr( "Failed To Unmount %1 Volume" ).arg( type ) ) ;

			this->enableAll() ;
		}
	}
}

void cryfsGUI::unMountAll()
{
	m_mountInfo.announceEvents( false ) ;

	this->disableAll() ;

	auto table = m_ui->tableWidget ;

	auto l = tablewidget::columnEntries( table,1 ) ;

	int r = l.size() - 1 ;

	if( r < 0 ){

		utility::Task::suspendForOneSecond() ;
	}else{
		do{
			const auto& e = l.at( r ) ;

			if( cryfsTask::encryptedFolderUnMount( e ).await() ){

				tablewidget::deleteRow( table,e,1 ) ;

				utility::Task::suspendForOneSecond() ;
			}

			r -= 1 ;

		}while( r >= 0 ) ;
	}

	this->enableAll() ;

	m_mountInfo.announceEvents( true ) ;
}

void cryfsGUI::pbUpdate()
{
	this->disableAll() ;

	tablewidget::clearTable( m_ui->tableWidget ) ;

	this->updateVolumeList( cryfsTask::updateVolumeList().await() ) ;
}

void cryfsGUI::updateVolumeList( const QVector< volumeInfo >& r )
{
	for( const auto& it : r ){

		if( it.entryisValid() ){

			this->updateList( it ) ;
		}
	}

	this->enableAll() ;
}

void cryfsGUI::slotCurrentItemChanged( QTableWidgetItem * current,QTableWidgetItem * previous )
{
	tablewidget::selectRow( current,previous ) ;
}

void cryfsGUI::disableAll()
{
	m_ui->pbmenu->setEnabled( false ) ;
	m_ui->pbupdate->setEnabled( false ) ;
	m_ui->tableWidget->setEnabled( false ) ;
	m_ui->pbunlockcryptfs->setEnabled( false ) ;
	m_ui->pbcreate->setEnabled( false ) ;
}

void cryfsGUI::enableAll()
{
	if( !m_removeAllVolumes ){

		m_ui->pbmenu->setEnabled( true ) ;
		m_ui->pbupdate->setEnabled( true ) ;
		m_ui->tableWidget->setEnabled( true ) ;
		m_ui->tableWidget->setFocus() ;
		m_ui->pbunlockcryptfs->setEnabled( true ) ;
		m_ui->pbcreate->setEnabled( true ) ;
	}
}

void cryfsGUI::enableAll_1()
{
	m_removeAllVolumes = false ;
	this->enableAll() ;
}

cryfsGUI::~cryfsGUI()
{
	if( m_ui ){

		auto q = m_ui->tableWidget ;

		const auto& r = this->window()->geometry() ;

		utility::setWindowDimensions( { r.x(),
						r.y(),
						r.width(),
						r.height(),
						q->columnWidth( 0 ),
						q->columnWidth( 1 ),
						q->columnWidth( 2 ),
						q->columnWidth( 3 ) } ) ;

		delete m_ui ;
	}
}
