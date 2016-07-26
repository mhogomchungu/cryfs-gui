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

#ifndef KEYDIALOG_H
#define KEYDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

#include <QMenu>

class QCloseEvent ;
class QAction ;
class QTableWidgetItem ;
class QTableWidget ;

#include "cryfs-gui.h"
#include "volumeinfo.h"
#include "utility.h"
#include "cryfstask.h"
#include "can_build_pwquality.h"

#include <functional>
#include <memory>

#if BUILD_PWQUALITY
class keystrength
{
public:
	keystrength() : m_handle( pwquality_default_settings() )
	{
	}
	~keystrength()
	{
		pwquality_free_settings( m_handle ) ;
	}
	int quality( const QString& e )
	{
		return pwquality_check( m_handle,e.toLatin1().constData(),
					nullptr,nullptr,nullptr ) ;
	}
	bool canCheckQuality()
	{
		return true ;
	}
private:
	pwquality_settings_t * m_handle ;
};
#else
class keystrength
{
public:
	keystrength()
	{
	}
	~keystrength()
	{
	}
	int quality( const QString& e )
	{
		Q_UNUSED( e ) ;
		return 0 ;
	}
	bool canCheckQuality()
	{
		return false ;
	}
private:
};
#endif

namespace Ui {
class keyDialog;
}

class keyDialog : public QDialog
{
	Q_OBJECT
public:
	static keyDialog& instance( QWidget * parent,
				     QTableWidget * table,
				     const volumeInfo& v,
				     std::function< void() > cancel,
				     std::function< void( const QString& ) > success )
	{
		return *( new keyDialog( parent,table,v,std::move( cancel ),std::move( success ) ) ) ;
	}
	keyDialog( QWidget * parent,
		   QTableWidget *,
		   const volumeInfo&,
		   std::function< void() >,
		   std::function< void( const QString& ) > ) ;
	~keyDialog() ;
	void ShowUI( void ) ;
	void HideUI( void ) ;
signals:
	void mounted( QString ) ;
	void cryptoOpen( QString ) ;
private slots:
	void textChanged( QString ) ;
	void passWordTextChanged( QString ) ;
	void cbActicated( int ) ;
	void pbOptions( void ) ;
	void pbkeyOption( void ) ;
	void pbMountPointPath( void ) ;
	void pbFolderPath( void ) ;
	void pbPluginEntryClicked( QAction * ) ;
	void plugIn( void ) ;
	void key( void ) ;
	void keyFile( void ) ;
	void pbOpen( void ) ;
	void pbCancel( void ) ;
	void Plugin( void ) ;
	void KeyFile( void ) ;
	void cbMountReadOnlyStateChanged( int ) ;
	void encryptedFolderMount( void ) ;
	void encryptedFolderCreate( void ) ;
private :
	bool completed( cryfsTask::status ) ;
	void keyAndKeyFile( void ) ;
	void openVolume( void ) ;
	void enableAll( void ) ;
	void disableAll( void ) ;
	void closeEvent( QCloseEvent * ) ;
	bool eventFilter( QObject * watched,QEvent * event ) ;
	Ui::keyDialog * m_ui ;
	QString m_path ;
	QString m_key ;
	QStringList m_keyFiles ;
	QString m_deviceOffSet ;
	QString m_options ;
        QString m_configFile ;
	QMenu * m_menu ;
	QTableWidget * m_table ;
	bool m_working ;
	bool m_create ;
	bool m_reUseMountPoint ;

	keystrength m_keyStrength ;

	typedef enum{ Key = 0,keyfile = 1,keyKeyFile = 2,plugin = 3 } keyType ;

	keyType m_keyType ;

	QWidget * m_parentWidget ;

	std::function< void() > m_cancel ;
	std::function< void( const QString& ) > m_success ;
};

#endif // KEYDIALOG_H
