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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QVector>

#include "volumeentryproperties.h"
#include "utility.h"

#include "lxqt_wallet/frontend/lxqt_wallet.h"

class QCloseEvent ;
class QAction ;
class QTableWidgetItem ;
class monitor_mountinfo ;

class changeWalletPassWord : public QWidget
{
	Q_OBJECT
public:
	static void instance( QWidget * parent )
	{
		new changeWalletPassWord( parent ) ;
	}
	changeWalletPassWord( QWidget * parent ) : m_wallet( LxQt::Wallet::getWalletBackend( LxQt::Wallet::internalBackEnd ) )
	{
		m_wallet->setInterfaceObject( this ) ;
		m_wallet->setParent( parent ) ;
		m_wallet->changeWalletPassWord( utility::walletName(),utility::applicationName() ) ;
	}
	~changeWalletPassWord()
	{
		m_wallet->deleteLater() ;
	}
private slots:
	void walletpassWordChanged( bool e )
	{
		Q_UNUSED( e ) ;
		this->deleteLater() ;
	}
	void walletIsOpen( bool e )
	{
		Q_UNUSED( e ) ;
	}	
private:
	LxQt::Wallet::Wallet * m_wallet ;
};

namespace Ui {
class cryfsGUI ;
}

class cryfsGUI : public QWidget
{
	Q_OBJECT
public:
	explicit cryfsGUI( QWidget * parent = 0 ) ;
	~cryfsGUI() ;
	void Show( void ) ;
signals:
	void result( int,QString ) ;
public slots:
	void raiseWindow( void ) ;
	void raiseWindow( QString ) ;
private slots:
	void aboutToShowMenu( void ) ;
	void changeInternalWalletPassWord( void ) ;
	void closeApplication( void ) ;
	void unlockCryptFs( void ) ;
	void startGUI( void ) ;
	void showMoungDialog( const volumeEntryProperties& ) ;
	void showMoungDialog( const QString&,const QString& = QString() ) ;
	void mount( const volumeEntryProperties& ) ;
	void defaultButton( void ) ;
	void itemClicked( QTableWidgetItem * ) ;
	void pbUpdate( void ) ;
	void pbCreate( void ) ;
	void slotMount( void ) ;
	void unMountAll( void ) ;
	void pbUmount( void ) ;
	void slotTrayClicked( QSystemTrayIcon::ActivationReason ) ;
	void slotCurrentItemChanged( QTableWidgetItem *,QTableWidgetItem * ) ;
	void enableAll( void ) ;
	void enableAll_1( void ) ;
	void slotOpenFolder( void ) ;
	void slotOpenSharedFolder( void ) ;
	void itemEntered( QTableWidgetItem * ) ;
	void addEntryToTable( bool,const QStringList& ) ;
	void addEntryToTable( bool,const volumeEntryProperties& ) ;
	void quitApplication( void ) ;
	void autoMountToggled( bool ) ;
	void autoOpenFolderOnMount( bool ) ;
	void removeEntryFromTable( QString ) ;
	void showFavorites( void ) ;
	void favoriteClicked( QAction * ) ;
	void keyManagerClicked( QAction * ) ;
	void openMountPointPath( QString ) ;
	void licenseInfo( void ) ;
	void updateCheck( void ) ;
	void languageMenu( QAction * ac ) ;
private:
	QString resolveFavoriteMountPoint( const QString& ) ;
	void autoUpdateCheck( void ) ;
	void updateVolumeList( const QVector< volumeEntryProperties >& ) ;
	void openMountPoint( const QString& ) ;
	QFont getSystemVolumeFont( void ) ;
	void setLocalizationLanguage( bool ) ;
	bool autoOpenFolderOnMount( void ) ;
	void dragEnterEvent( QDragEnterEvent * ) ;
	void dropEvent( QDropEvent * ) ;
	void showContextMenu( QTableWidgetItem *,bool ) ;
	void startAutoMonitor( void ) ;
	bool autoMount( void ) ;
	void updateList( const volumeEntryProperties& ) ;
	void setupKeyManager( QMenu * ) ;

	Ui::cryfsGUI * m_ui = nullptr ;
	QString m_folderOpener ;
	void disableAll( void ) ;
	void closeEvent( QCloseEvent * e ) ;
	void setUpFont( void ) ;
	void setUpShortCuts( void ) ;
	void setUpApp( const QString& ) ;

	QAction * m_autoMountAction = nullptr ;
	QMenu * m_favorite_menu = nullptr ;
	QMenu * m_hidden_volume_menu = nullptr ;
	QMenu * m_not_hidden_volume_menu = nullptr ;
	QMenu * m_key_manager_menu = nullptr ;

	QAction * m_change_password_action = nullptr ;

	QAction * m_languageAction = nullptr ;

	bool m_startHidden ;
	bool m_autoMount ;
	QString m_sharedFolderPath ;
	bool m_autoOpenFolderOnMount ;
	bool m_removeAllVolumes = false ;
	QString m_env ;

	QSystemTrayIcon m_trayIcon ;

	monitor_mountinfo& m_mountInfo ;
};

#endif // MAINWINDOW_H
