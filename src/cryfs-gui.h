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

#include "volumeinfo.h"
#include "utility.h"
#include "secrets.h"

#include "lxqt_wallet.h"

class QCloseEvent ;
class QAction ;
class QTableWidgetItem ;
class monitor_mountinfo ;

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
public slots:
	void raiseWindow( void ) ;
	void raiseWindow( QString ) ;
private slots:
	void setDefaultMountPointPrefix( void ) ;
	void showTrayGUI( void ) ;
	void properties( void ) ;
	void autoCheckUpdates( bool ) ;
	void reuseMountPoint( bool ) ;
        void unlockVolume( const QString&,const QString&,const QString&,const QString&,const QString&,bool ) ;
	void aboutToShowMenu( void ) ;
	void changeInternalWalletPassWord( void ) ;
	void closeApplication( void ) ;
	void unlockCryptFs( void ) ;
	void startGUI( void ) ;
	void showMoungDialog( const volumeInfo& ) ;
	void showMoungDialog( const QString&,const QString& = QString() ) ;
	void mount( const volumeInfo&,const QString& = QString() ) ;
	void defaultButton( void ) ;
	void itemClicked( QTableWidgetItem * ) ;
	void pbUpdate( void ) ;
	void createVolume( QAction * = nullptr ) ;
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
	void addEntryToTable( const QStringList& ) ;
	void addEntryToTable( const volumeInfo& ) ;
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

	QFont getSystemVolumeFont( void ) ;

	bool autoOpenFolderOnMount( void ) ;

	void autoUpdateCheck( void ) ;
	void updateVolumeList( const QVector< volumeInfo >& ) ;
	void openMountPoint( const QString& ) ;
	void setLocalizationLanguage( bool ) ;

	void dragEnterEvent( QDragEnterEvent * ) ;
	void dropEvent( QDropEvent * ) ;
	void showContextMenu( QTableWidgetItem *,bool ) ;
	void startAutoMonitor( void ) ;
	void updateList( const volumeInfo& ) ;
	void setUpAppMenu( void ) ;
	void disableAll( void ) ;
	void closeEvent( QCloseEvent * e ) ;
	void setUpFont( void ) ;
	void setUpShortCuts( void ) ;
	void setUpApp( const QString& ) ;

	Ui::cryfsGUI * m_ui = nullptr ;

	secrets m_secrets ;

	QMenu * m_favorite_menu = nullptr ;
	QMenu * m_hidden_volume_menu = nullptr ;
	QMenu * m_not_hidden_volume_menu = nullptr ;
	QMenu * m_key_manager_menu = nullptr ;
	QMenu * m_language_menu = nullptr ;

	QVector< std::pair< QAction *,const char * > > m_actionPair ;
	QVector< std::pair< QMenu *,const char * > > m_menuPair ;

	QAction * m_unMountAll = nullptr ;
	QAction * m_change_password_action = nullptr ;

	bool m_startHidden ;
	bool m_autoOpenFolderOnMount ;
	bool m_removeAllVolumes = false ;

	QString m_env ;
	QString m_sharedFolderPath ;
	QString m_folderOpener ;

	QSystemTrayIcon m_trayIcon ;

	monitor_mountinfo& m_mountInfo ;
};

#endif // MAINWINDOW_H
