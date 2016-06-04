/*
 *
 *  Copyright ( c ) 2011-2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MISCFUNCTIONS_H
#define MISCFUNCTIONS_H

#include <QString>
#include <QStringList>
#include <QEvent>
#include <QProcess>
#include <QThreadPool>
#include <QRunnable>
#include <QMetaObject>
#include <QDebug>
#include <QWidget>
#include <QDialog>
#include <QEventLoop>
#include <QTimer>
#include <QMenu>
#include <QVector>
#include <QSystemTrayIcon>
#include <QAction>
#include <QIcon>

#include <functional>
#include <memory>
#include <array>
#include <utility>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "task.h"
#include "lxqt_wallet/frontend/lxqt_wallet.h"

#include <QObject>
#include <QLabel>

#include <poll.h>
#include <fcntl.h>

class QByteArray ;
class QEvent ;

namespace utility
{
	class selectMenuOption : public QObject
	{
		Q_OBJECT
	public:
		using function_t = std::function< void( const QString& e ) > ;

		selectMenuOption( QMenu * m,bool e,
				  function_t && f = []( const QString& e ){ Q_UNUSED( e ) } ) :
		m_menu( m ),m_function( f )
		{
			if( e ){

				this->setParent( m ) ;
			}
		}
	public slots :
		void selectOption( const QString& f )
		{
			for( const auto& it : m_menu->actions() ){

				QString e = it->text() ;

				e.remove( "&" ) ;

				it->setChecked( f == e ) ;
			}

			m_function( f ) ;
		}
		void selectOption( QAction * ac )
		{
			auto e = ac->text() ;

			e.remove( "&" ) ;

			this->selectOption( e ) ;
		}
	private:
		QMenu * m_menu ;
		std::function< void( const QString& ) > m_function ;
	};
}

namespace utility
{
	struct wallet
	{
		bool opened ;
		bool notConfigured ;
		QString key ;
		QString password ;
	};

	int startApplication( const char * appName,std::function<int()> ) ;

	wallet getKeyFromWallet( LxQt::Wallet::walletBackEnd,const QString& keyID,const QString& pwd = QString() ) ;

	QString cmdArgumentValue( const QStringList&,const QString& arg,const QString& defaulT = QString() ) ;

	QIcon getIcon() ;

	QStringList directoryList( const QString& e ) ;

	QString executableFullPath( const QString& ) ;

	bool reUseMountPoint( void ) ;
	void reUseMountPoint( bool ) ;

	QString homeConfigPath( const QString& = QString() ) ;
	QString homePath() ;
	QString mountPath( const QString& path ) ;

	QString mountPathPostFix( const QString& path ) ;
	QString mountPathPostFix( const QString& prefix,const QString& path ) ;

	bool pathIsReadable( const QString& ) ;

	bool setOpenVolumeReadOnly( QWidget * parent,bool check ) ;
	bool getOpenVolumeReadOnlyOption() ;

	QStringList split( const QString&,char = '\n' ) ;
	void addToFavorite( const QString& dev,const QString& m_point ) ;

	QStringList readFavorites( void ) ;
	void readFavorites( QMenu *,bool = false ) ;
	void removeFavoriteEntry( const QString& ) ;
	QString getVolumeID( const QString&,bool = false ) ;
	QString localizationLanguage() ;
	QString localizationLanguagePath() ;
	void setLocalizationLanguage( const QString& ) ;
	QString walletName( void ) ;
	QString applicationName( void ) ;
	bool eventFilter( QObject * gui,QObject * watched,QEvent * event,std::function< void() > ) ;
	void licenseInfo( QWidget * ) ;
	void showTrayIcon( QAction *,QSystemTrayIcon *,bool = true ) ;
	void trayProperty( QSystemTrayIcon *,bool = true ) ;

	void setLocalizationLanguage( bool translate,QMenu * m ) ;
	void languageMenu( QWidget *,QMenu *,QAction * ) ;

	using array_t = std::array< int,8 > ;

	utility::array_t getWindowDimensions() ;
	void setWindowDimensions( const std::initializer_list<int>& ) ;

	void createPlugInMenu( QMenu *,const QString&,const QString&,const QString& ) ;

	int pluginKey( QDialog *,QString *,const QString& ) ;

	QFont getFont( QWidget * ) ;
	void saveFont( const QFont& ) ;

	::Task::future<QString>& getKeyFromWallet( LxQt::Wallet::Wallet * wallet,const QString& volumeID ) ;
	::Task::future< bool >& openPath( const QString& path,const QString& opener,const QString& env = QString() ) ;

	void openPath( const QString& path,const QString& opener,const QString& env,QWidget *,const QString&,const QString& ) ;
}

namespace utility
{
	bool pathExists( const QString& ) ;

	template< typename ... F >
	bool atLeastOnePathExists( const F& ... f ){

		for( const auto& it : { f ... } ){

			if( utility::pathExists( it ) ){

				return true ;
			}
		}

		return false ;
	}

	template< typename E,typename ... F >
	bool containsAtleastOne( const E& e,const F& ... f )
	{
		for( const auto& it : { f ... } ){

			if( e.contains( it ) ){

				return true ;
			}
		}

		return false ;
	}

	template< typename E,typename ... F >
	bool startsWithAtLeastOne( const E& e,const F& ... f )
	{
		for( const auto& it : { f ... } ){

			if( e.startsWith( it ) ){

				return true ;
			}
		}

		return false ;
	}

        template< typename E,typename ... F >
        bool endsWithAtLeastOne( const E& e,const F& ... f )
        {
                for( const auto& it : { f ... } ){

                        if( e.endsWith( it ) ){

                                return true ;
                        }
                }

                return false ;
        }
}

namespace utility
{
	class fileHandle
	{
	public:
		fileHandle()
		{
		}
		fileHandle( int r ) : m_fd( r )
		{
		}
		fileHandle( int r,std::function< void( int ) > cmd ) :
			m_fd( r ),m_releaseResource( std::move( cmd ) )
		{
		}
		bool open( const char * filePath,bool ro = true )
		{
			if( ro ){

				m_fd = ::open( filePath,O_RDONLY ) ;
			}else{
				m_fd = ::open( filePath,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH ) ;
			}

			m_path = filePath ;

			return m_fd != -1 ;
		}
		bool open( const QString& filePath,bool ro = true )
		{
			return this->open( filePath.toLatin1().constData(),ro ) ;
		}
		int handle() const
		{
			return m_fd ;
		}
		const char * path()
		{
			return m_path.constData() ;
		}
		bool opened() const
		{
			return m_fd != -1 ;
		}
		~fileHandle()
		{
			m_releaseResource( m_fd ) ;
		}
	private:
		int m_fd = -1 ;

		QByteArray m_path ;

		std::function< void( int ) > m_releaseResource = []( int fd ){

			if( fd != -1 ){

				::close( fd ) ;
			}
		} ;
	} ;
}

namespace utility
{
	class monitor_mountinfo
	{
	public:
		static QStringList mountinfo()
		{
			QFile f( "/proc/self/mountinfo" ) ;

			QString e ;

			if( f.open( QIODevice::ReadOnly ) ){

				e = f.readAll() ;
			}

			return utility::split( e ) ;
		}

		monitor_mountinfo()
		{
			m_handle.open( "/proc/self/mountinfo" ) ;
			m_monitor.fd     = m_handle.handle() ;
			m_monitor.events = POLLPRI ;
		}
		operator bool()
		{
			return m_handle.opened() ;
		}
		bool gotEvent() const
		{
			poll( &m_monitor,1,-1 ) ;
			return true ;
		}
	private:
		utility::fileHandle m_handle ;
		mutable struct pollfd m_monitor ;
	};
}

namespace utility
{
	class Task
	{
	public :
		static ::Task::future< utility::Task >& run( const QString& exe )
		{
			return ::Task::run< utility::Task >( [ exe ](){ return utility::Task( exe ) ; } ) ;
		}
		static void wait( int s )
		{
			sleep( s ) ;
		}
		static void waitForOneSecond( void )
		{
			sleep( 1 ) ;
		}
		static void waitForTwoSeconds( void )
		{
			sleep( 2 ) ;
		}
		static void suspendForOneSecond( void )
		{
			utility::Task::suspend( 1 ) ;
		}
		static void suspend( int s )
		{
			QTimer t ;

			QEventLoop l ;

			QObject::connect( &t,SIGNAL( timeout() ),&l,SLOT( quit() ) ) ;

			t.start( 1000 * s ) ;

			l.exec() ;
		}
		Task()
		{
		}
		Task( const QString& exe,int waitTime = -1,const QStringList& env = QStringList(),
		      std::function< void() > f = [](){} )
		{
			class Process : public QProcess{
			public:
				Process( std::function< void() >&& f ) : m_function( std::move( f ) )
				{
				}
			protected:
				void setupChildProcess()
				{
					m_function() ;
				}
			private:
				std::function< void() > m_function ;
			} p( std::move( f ) ) ;

			p.setEnvironment( env ) ;
			p.start( exe ) ;
			m_finished   = p.waitForFinished( waitTime ) ;
			m_exitCode   = p.exitCode() ;
			m_exitStatus = p.exitStatus() ;
			m_data       = p.readAll() ;
		}
		QStringList splitOutput( char token ) const
		{
			return QString( m_data ).split( token,QString::SkipEmptyParts ) ;
		}
		void output( const QByteArray& r )
		{
			m_data = r ;
		}
		const QByteArray& output() const
		{
			return m_data ;
		}
		int exitCode() const
		{
			return m_exitCode ;
		}
		int exitStatus() const
		{
			return m_exitStatus ;
		}
		bool success() const
		{
			return m_exitCode == 0 && m_exitStatus == QProcess::NormalExit && m_finished == true ;
		}
		bool failed() const
		{
			return !this->success() ;
		}
		bool finished() const
		{
			return m_finished ;
		}
		bool ok() const
		{
			return this->splitOutput( '\n' ).size() > 12 ;
		}
	private:
		QByteArray m_data ;
		int m_exitCode ;
		int m_exitStatus ;
		bool m_finished ;
	};
}

#endif // MISCFUNCTIONS_H
