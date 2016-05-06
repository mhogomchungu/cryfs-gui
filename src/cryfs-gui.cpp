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

	auto f = utility::getWindowDimensions( "cryfs" ) ;

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

	this->setUpShortCuts() ;

	this->setUpFont() ;

	const auto& icon = utility::getIcon( "cryfs-gui" ) ;

	this->setAcceptDrops( true ) ;
	this->setWindowIcon( icon ) ;

	m_trayIcon.setParent( this ) ;
	m_trayIcon.setIcon( icon ) ;

	auto trayMenu = new QMenu( this ) ;

	trayMenu->setFont( this->font() ) ;

	m_autoOpenFolderOnMount = this->autoOpenFolderOnMount() ;
	m_autoMount = this->autoMount() ;

	trayMenu->addAction( [ this ](){

		auto ac = new QAction( tr( "Auto Open Mount Point" ),this ) ;

		ac->setCheckable( true ) ;
		ac->setChecked( m_autoOpenFolderOnMount ) ;

		connect( ac,SIGNAL( toggled( bool ) ),this,SLOT( autoOpenFolderOnMount( bool ) ) ) ;

		return ac ;
	}() ) ;

	trayMenu->addAction( [ this ](){

		auto ac = new QAction( tr( "Unmount All" ),this ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( unMountAll() ) ) ;

		return ac ;
	}() ) ;

	this->setupKeyManager( trayMenu ) ;

	m_favorite_menu = trayMenu->addMenu( tr( "Favorites" ) ) ;

	m_favorite_menu->setFont( this->font() ) ;

	connect( m_favorite_menu,SIGNAL( triggered( QAction * ) ),
		 this,SLOT( favoriteClicked( QAction * ) ) ) ;

	connect( m_favorite_menu,SIGNAL( aboutToShow() ),
		 this,SLOT( showFavorites() ) ) ;

	m_languageAction = new QAction( tr( "Select Language" ),this ) ;

	trayMenu->addAction( m_languageAction ) ;

	trayMenu->addAction( [ this ](){

		auto ac = new QAction( tr( "Check For Update" ),this ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( updateCheck() ) ) ;

		return ac ;
	}() ) ;

	trayMenu->addAction( [ this ](){

		auto ac = new QAction( tr( "About" ),this ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( licenseInfo() ) ) ;

		return ac ;
	}() ) ;

	trayMenu->addAction( tr( "Quit" ),this,SLOT( closeApplication() ) ) ;

	m_trayIcon.setContextMenu( trayMenu ) ;

	connect( &m_trayIcon,SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
		 this,SLOT( slotTrayClicked( QSystemTrayIcon::ActivationReason ) ) ) ;

	m_ui->pbmenu->setMenu( m_trayIcon.contextMenu() ) ;

	this->setLocalizationLanguage( false ) ;

	m_trayIcon.show() ;

	auto dirPath = utility::homePath() + "/.cryfs-gui/" ;
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

void cryfsGUI::aboutToShowMenu()
{
	m_change_password_action->setEnabled( LxQt::Wallet::walletExists( LxQt::Wallet::internalBackEnd,utility::walletName(),utility::applicationName() ) ) ;
}

void cryfsGUI::setupKeyManager( QMenu * m )
{
	m_key_manager_menu = new QMenu( tr( "Key Storage" ),this ) ;

	connect( m_key_manager_menu,SIGNAL( triggered( QAction * ) ),
		 this,SLOT( keyManagerClicked( QAction * ) ) ) ;

	connect( m_key_manager_menu,SIGNAL( aboutToShow() ),this,SLOT( aboutToShowMenu() ) ) ;

	auto w = new QMenu( tr( "Internal Wallet" ),this ) ;

	m_change_password_action = w->addAction( tr( "Change PassWord" ) ) ;

	connect( m_change_password_action,SIGNAL( triggered() ),this,SLOT( changeInternalWalletPassWord() ) ) ;

	m->addMenu( w ) ;

	auto i = LxQt::Wallet::backEndIsSupported( LxQt::Wallet::internalBackEnd ) ;
	auto k = LxQt::Wallet::backEndIsSupported( LxQt::Wallet::kwalletBackEnd ) ;
	auto g = LxQt::Wallet::backEndIsSupported( LxQt::Wallet::secretServiceBackEnd ) ;

	m_key_manager_menu->addAction( tr( "Internal Wallet" ) )->setEnabled( i ) ;
	m_key_manager_menu->addAction( tr( "KDE Wallet" ) )->setEnabled( k ) ; ;
	m_key_manager_menu->addAction( tr( "Gnome Wallet" ) )->setEnabled( g ) ; ;

	m->addMenu( m_key_manager_menu ) ;
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
	checkForUpdates::instance( this ) ;
}

void cryfsGUI::autoUpdateCheck()
{
	checkForUpdates::instance( this,"cryfs-gui" ) ;
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
	utility::setLocalizationLanguage( translate,this,m_languageAction,"cryfs-gui" ) ;
}

void cryfsGUI::languageMenu( QAction * ac )
{
	utility::languageMenu( this,m_languageAction->menu(),ac,"cryfs-gui" ) ;
}

#define zuluMOUNT_AUTO_OPEN_FOLDER "/.cryfs-gui/cryfs-gui.NoAutoOpenFolder"

void cryfsGUI::autoOpenFolderOnMount( bool b )
{
	auto x = utility::homePath() + zuluMOUNT_AUTO_OPEN_FOLDER ;

	m_autoOpenFolderOnMount = b ;

	if( m_autoOpenFolderOnMount ){

		QFile::remove( x ) ;
	}else{
		QFile f( x ) ;
		f.open( QIODevice::WriteOnly ) ;
		f.close() ;
	}
}

bool cryfsGUI::autoOpenFolderOnMount( void )
{
	auto x = utility::homePath() + zuluMOUNT_AUTO_OPEN_FOLDER ;
	return !QFile::exists( x ) ;
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

	oneinstance::instance( this,"cryfs-gui.socket","startGUI",volume,[ this,volume ]( QObject * instance ){

		connect( instance,SIGNAL( raise() ),this,SLOT( raiseWindow() ) ) ;
		connect( instance,SIGNAL( raiseWithDevice( QString ) ),this,SLOT( raiseWindow( QString ) ) ) ;

		this->setUpApp( volume ) ;
	} ) ;
}

void cryfsGUI::showContextMenu( QTableWidgetItem * item,bool itemClicked )
{
	QMenu m ;

	m.setFont( this->font() ) ;

	auto row = item->row() ;

	auto mt = m_ui->tableWidget->item( row,1 )->text() ;

	if( mt == "Nil" ){

		connect( m.addAction( tr( "Mount" ) ),SIGNAL( triggered() ),this,SLOT( slotMount() ) ) ;
	}else{
		auto home = utility::homePath().split( '/').last() ;

		auto mp   = QString( "/run/media/private/%1/" ).arg( home ) ;
		auto mp_1 = QString( "/home/%1/" ).arg( home ) ;

		if( mt.startsWith( mp ) || mt.startsWith( mp_1 ) ){

			connect( m.addAction( tr( "Unmount" ) ),SIGNAL( triggered() ),this,SLOT( pbUmount() ) ) ;

			m.addSeparator() ;

			if( m_sharedFolderPath.isEmpty() ){

				connect( m.addAction( tr( "Open Folder" ) ),SIGNAL( triggered() ),
					 this,SLOT( slotOpenFolder() ) ) ;
			}else{
				connect( m.addAction( tr( "Open Private Folder" ) ),SIGNAL( triggered() ),
					 this,SLOT( slotOpenFolder() ) ) ;
				connect( m.addAction( tr( "Open Shared Folder" ) ),SIGNAL( triggered() ),
					 this,SLOT( slotOpenSharedFolder() ) ) ;
			}
		}else{
			if( m_sharedFolderPath.isEmpty() ){

				if( utility::pathIsReadable( mt ) ){

					connect( m.addAction( tr( "Open Folder" ) ),SIGNAL( triggered() ),
						 this,SLOT( slotOpenFolder() ) ) ;
				}
			}else{
				connect( m.addAction( tr( "Open Shared Folder" ) ),SIGNAL( triggered() ),
					 this,SLOT( slotOpenSharedFolder() ) ) ;
			}
		}
	}

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
	auto row = m_ui->tableWidget->currentRow() ;
	auto mt = m_ui->tableWidget->item( row,1 )->text() ;

	if( mt == "Nil" ){

		this->slotMount() ;
	}else{
		this->showContextMenu( m_ui->tableWidget->currentItem(),false ) ;
	}
}

void cryfsGUI::slotOpenSharedFolder()
{
	this->openMountPoint( m_sharedFolderPath ) ;
}

void cryfsGUI::slotOpenFolder()
{
	auto item = m_ui->tableWidget->currentItem() ;
	auto path = m_ui->tableWidget->item( item->row(),1 )->text() ;

	this->openMountPoint( path ) ;
}

void cryfsGUI::openMountPoint( const QString& m_point )
{
	auto x = tr( "Warning" ) ;
	auto y = tr( "Could not open mount point because \"%1\" tool does not appear to be working correctly").arg( m_folderOpener ) ;

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
	this->addAction( [ this ](){

		auto ac = new QAction( this ) ;

		QList<QKeySequence> keys ;

		keys.append( Qt::Key_Enter ) ;
		keys.append( Qt::Key_Return ) ;

		ac->setShortcuts( keys ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( defaultButton() ) ) ;

		return ac ;
	}() ) ;

	this->addAction( [ this ](){

		auto ac = new QAction( this ) ;

		QList<QKeySequence> z ;

		z.append( Qt::Key_M ) ;

		ac->setShortcuts( z ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( pbCreate() ) ) ;

		return ac ;
	}() ) ;

	this->addAction( [ this ](){

		auto ac = new QAction( this ) ;

		QList<QKeySequence> p ;

		p.append( Qt::Key_U ) ;

		ac->setShortcuts( p ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( pbUmount() ) ) ;

		return ac ;
	}() ) ;

	this->addAction( [ this ](){

		auto ac = new QAction( this ) ;

		QList<QKeySequence> q ;

		q.append( Qt::Key_R ) ;
		ac->setShortcuts( q ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( pbUpdate() ) ) ;

		return ac ;
	}() ) ;

	this->addAction( [ this ](){

		auto ac = new QAction( this ) ;

		QList<QKeySequence> e ;

		e.append( Qt::Key_C ) ;
		ac->setShortcuts( e ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( closeApplication() ) ) ;

		return ac ;
	}() ) ;
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

void cryfsGUI::autoMountToggled( bool opt )
{
	m_autoMount = opt ;
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
	auto row = table->currentRow() ;

	this->mount( tablewidget::tableRowEntries( table,row ) ) ;
}

void cryfsGUI::showMoungDialog( const volumeInfo& v )
{
	if( v.isEmpty() ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR" ),
			      tr( "Permission to access the volume was denied\nor\nthe volume is not supported" ) ) ;

		this->enableAll() ;
	}else{
		this->mount( v ) ;
	}
}

void cryfsGUI::showMoungDialog( const QString& volume,const QString& m_point )
{
	if( !volume.isEmpty() ){

		this->mount( { volume,m_point,"cryptfs","Nil","Nil","Nil" } ) ;
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
	tablewidget::addRowToTable( m_ui->tableWidget,l ) ;
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

		tablewidget::deleteRowFromTable( table,r ) ;
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

			row = tablewidget::addEmptyRow( table ) ;
		}

		tablewidget::updateRowInTable( table,entry.entryList(),row,this->font() ) ;

		tablewidget::selectRow( table,row ) ;
	}
}

void cryfsGUI::pbUmount()
{
	this->disableAll() ;

	auto row = m_ui->tableWidget->currentRow() ;

	auto type = m_ui->tableWidget->item( row,2 )->text() ;

	auto m = m_ui->tableWidget->item( row,1 )->text() ;

	if( !cryfsTask::encryptedFolderUnMount( m ).await() ){

		DialogMsg m( this ) ;
		m.ShowUIOK( tr( "ERROR" ),tr( "Failed to unmount %1 volume" ).arg( type ) ) ;

		this->enableAll() ;
	}
}

void cryfsGUI::unMountAll()
{
	m_mountInfo.announceEvents( false ) ;

	this->disableAll() ;

	auto table = m_ui->tableWidget ;

	auto l = tablewidget::tableColumnEntries( table,1 ) ;

	auto r = l.size() - 1 ;

	if( r < 0 ){

		utility::suspend( 1 ) ;
	}else{
		for( int i = r ; i >= 0 ; i-- ){

			const auto& e = l.at( i ) ;

			if( cryfsTask::encryptedFolderUnMount( e ).await() ){

				tablewidget::deleteTableRow( table,e,1 ) ;

				utility::suspend( 1 ) ;
			}
		}
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
	tablewidget::selectTableRow( current,previous ) ;
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

#define zuluMOUNT_AUTOPATH "/.zuluCrypt/cryfsGUI-gui.autoMountPartitions"

bool cryfsGUI::autoMount()
{
	QFile f( utility::homePath() + zuluMOUNT_AUTOPATH ) ;
	return f.exists() ;
}

cryfsGUI::~cryfsGUI()
{
	QFile f( utility::homePath() + zuluMOUNT_AUTOPATH ) ;

	auto q = m_ui->tableWidget ;

	const auto& r = this->window()->geometry() ;

	utility::setWindowDimensions( "cryfs",{ r.x(),
						r.y(),
						r.width(),
						r.height(),
						q->columnWidth( 0 ),
						q->columnWidth( 1 ),
						q->columnWidth( 2 ),
						q->columnWidth( 3 ) } ) ;

	delete m_ui ;
}
