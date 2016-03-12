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

#include "utility.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <QObject>
#include <QDir>
#include <pwd.h>
#include <grp.h>

#include <memory>

#include <QTranslator>
#include <QEventLoop>
#include <QDebug>
#include <QCoreApplication>
#include <blkid/blkid.h>
#include <QByteArray>
#include <QProcess>
#include <QFile>
#include <QFile>
#include <QDir>
#include <QTableWidget>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QProcessEnvironment>
#include <unistd.h>
#include <pwd.h>

#include <QEvent>
#include <QKeyEvent>

#include "install_prefix.h"
#include "locale_path.h"

#include "plugins.h"

#include "readonlywarning.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <gcrypt.h>

#include "lxqt_wallet/frontend/lxqt_wallet.h"

#include "reuse_mount_point.h"

#include "dialogmsg.h"
#include "plugin.h"

#include "version.h"

static int _help()
{
	std::cout << "\n" << VERSION_STRING << std::endl ;

	QString helpMsg = QObject::tr( "\n\
options:\n\
	-d   path to where a volume to be auto unlocked/mounted is located\n\
	-m   tool to use to open a default file manager(default tool is xdg-open)\n\
	-e   start the application without showing the GUI\n" ) ;

	std::cout << helpMsg.toLatin1().constData() << std::endl ;

	return 0 ;
}

static bool _printHelpOrVersionInfo()
{
	QStringList q = QCoreApplication::arguments() ;
	return q.contains( "-h" )        ||
	       q.contains( "-help" )     ||
	       q.contains( "--help" )    ||
	       q.contains( "-v" )        ||
	       q.contains(  "-version" ) ||
	       q.contains( "--version" ) ;
}

int utility::startApplication( const char * appName,std::function<int()> start )
{
	QCoreApplication::setApplicationName( appName ) ;

	if( _printHelpOrVersionInfo() ){

		return _help() ;
	}else{
		return start() ;
	}
}

static bool _execute_process( const QString& m,const QString& exe,const QString& env,int uid )
{
	if( exe.startsWith( "/" ) && utility::pathExists( exe ) ){

		auto e = m ;

		e.replace( "\"","\"\"\"" ) ;

		return utility::Task( exe + " \"" + m + "\"",-1,env.split( "\n" ),[ uid ](){

			if( uid != -1 ){

				Q_UNUSED( setgid( uid ) ) ;
				Q_UNUSED( setgroups( 1,reinterpret_cast< const gid_t * >( &uid ) ) ) ;
				Q_UNUSED( setegid( uid ) ) ;
				Q_UNUSED( setuid( uid ) ) ;

				auto id = getpwuid( uid ) ;

				if( id ){

					setenv( "LOGNAME",id->pw_name,1 ) ;
					setenv( "HOME",id->pw_dir,1 ) ;
					setenv( "USER",id->pw_name,1 ) ;
				}
			}

		} ).success() ;
	}else{
		return false ;
	}
}

::Task::future<bool>& utility::openPath( const QString& path,const QString& opener,const QString& env )
{
	return ::Task::run<bool>( [ env,path,opener ](){

		return _execute_process( path,opener,env,-1 ) == false ;
	} ) ;
}

void utility::openPath( const QString& path,const QString& opener,const QString& env,QWidget * obj,const QString& title,const QString& msg )
{
	openPath( path,opener,env ).then( [ title,msg,obj ]( bool failed ){

		if( failed && obj ){

			DialogMsg m( obj ) ;
			m.ShowUIOK( title,msg ) ;
		}
	} ) ;
}

::Task::future<QString>& utility::getKeyFromWallet( LxQt::Wallet::Wallet * wallet,const QString& volumeID )
{
	return ::Task::run<QString>( [ wallet,volumeID ](){

		return 	wallet->readValue( volumeID ) ;
	} ) ;
}

utility::wallet utility::getKeyFromWallet( LxQt::Wallet::walletBackEnd storage,const QString& keyID,const QString& pwd )
{
	utility::wallet w{ false,false,"","" } ;

	auto _getBackEnd = []( LxQt::Wallet::walletBackEnd e ){

		return LxQt::Wallet::getWalletBackend( e ) ;
	} ;

	using storage_t = std::unique_ptr< LxQt::Wallet::Wallet > ;

	if( storage == LxQt::Wallet::kwalletBackEnd ){

		storage_t e( _getBackEnd( storage ) ) ;

		w.opened = e->await_open( "default",utility::applicationName() ) ;

		if( w.opened ){

			w.key = utility::getKeyFromWallet( e.get(),keyID ).await() ;
		}

		return w ;

	}else if( storage == LxQt::Wallet::internalBackEnd ){

		auto walletName = utility::walletName() ;
		auto appName    = utility::applicationName() ;

		if( LxQt::Wallet::walletExists( LxQt::Wallet::internalBackEnd,walletName,appName ) ){

			storage_t e( _getBackEnd( storage ) ) ;

			e->setImage( ":/zuluCrypt.png" ) ;

			w.opened = e->await_open( walletName,appName,pwd ) ;

			if( w.opened ){

				w.key = utility::getKeyFromWallet( e.get(),keyID ).await() ;
				w.password = e->qObject()->objectName() ;
				w.notConfigured = false ;
			}

			return w ;
		}else{
			w.notConfigured = true ;
			return w ;
		}

	}else if( storage == LxQt::Wallet::secretServiceBackEnd ){

		storage_t e( _getBackEnd( storage ) ) ;

		w.opened = e->await_open( utility::walletName(),utility::applicationName() ) ;

		if( w.opened ){

			w.key = utility::getKeyFromWallet( e.get(),keyID ).await() ;
		}

		return w ;
	}else{
		/*
		 * shouldnt get here
		 */
		return w ;
	}
}

bool utility::eventFilter( QObject * gui,QObject * watched,QEvent * event,std::function< void() > function )
{
	if( watched == gui ){

		if( event->type() == QEvent::KeyPress ){

			auto keyEvent = static_cast< QKeyEvent* >( event ) ;

			if( keyEvent->key() == Qt::Key_Escape ){

				function() ;

				return true ;
			}
		}
	}

	return false ;
}

static QString _absolute_exe_path( const QString& exe )
{
	QString e = "/usr/local/bin/" + exe ;

	if( utility::pathExists( e ) ){

		return e ;
	}

	e = "/usr/local/sbin/" + exe ;

	if( utility::pathExists( e ) ){

		return e ;
	}

	e = "/usr/bin/" + exe ;

	if( utility::pathExists( e ) ){

		return e ;
	}

	e = "/usr/sbin/" + exe ;

	if( utility::pathExists( e ) ){

		return e ;
	}

	e = "/bin/" + exe ;

	if( utility::pathExists( e ) ){

		return e ;
	}

	e = "/sbin/" + exe ;

	if( utility::pathExists( e ) ){

		return e ;
	}

	return exe ;
}

QString utility::cmdArgumentValue( const QStringList& l,const QString& arg,const QString& defaulT )
{
	int j = l.size() ;

	for( int i = 0 ; i < j ; i++ ){

		if( l.at( i ) == arg ){

			auto e = [ & ](){

				if( i + 1 < j ){

					return l.at( i + 1 ) ;
				}else{
					return defaulT ;
				}
			} ;

			if( arg == "-m" ){

				return _absolute_exe_path( e() ) ;
			}else{
				return e() ;
			}
		}
	}

	if( defaulT == "xdg-open" ){

		return _absolute_exe_path( defaulT ) ;
	}else{
		return defaulT ;
	}
}

QString utility::getVolumeID( const QString& id,bool expand )
{
	Q_UNUSED( expand ) ;
	return id ;
}

void utility::addToFavorite( const QString& dev,const QString& m_point )
{
	if( !( dev.isEmpty() || m_point.isEmpty() ) ){

		auto fav = QString( "%1\t%2\n" ).arg( dev,m_point ) ;

		QFile f( utility::homePath() + "/.cryfs-gui/favorites" ) ;

		f.open( QIODevice::WriteOnly | QIODevice::Append ) ;

		f.write( fav.toLatin1() ) ;
	}
}

QStringList utility::readFavorites()
{
	QFile f( utility::homePath() + "/.cryfs-gui/favorites" ) ;

	if( f.open( QIODevice::ReadOnly ) ){

		QStringList l ;

		for( const auto& it : QString( f.readAll() ).split( '\n',QString::SkipEmptyParts ) ){

			l.append( it ) ;
		}

		return l ;
	}else{
		return QStringList() ;
	}
}

void utility::removeFavoriteEntry( const QString& entry )
{
	auto l = utility::readFavorites() ;

	l.removeOne( entry ) ;

	QFile f( utility::homePath() + "/.cryfs-gui/favorites" ) ;

	f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ;

	for( const auto& it : l ){

		auto e = it.split( '\t' ) ;

		auto q = QString( "%1\t%2\n" ).arg( e.at( 0 ),e.at( 1 ) ) ;

		f.write( q.toLatin1() ) ;
	}
}

void utility::readFavorites( QMenu * m,bool truncate )
{
	m->clear() ;

	auto _add_action = [ m,truncate ]( const QString& e ){

		auto ac = new QAction( m ) ;

		if( truncate ){

			auto l = utility::split( e,'\t' ) ;

			if( l.size() > 0 ){

				ac->setText( l.first() ) ;
			}
		}else{
			ac->setText( e ) ;
		}

		ac->setEnabled( !e.startsWith( "/dev/disk/by-id" ) ) ;

		return ac ;
	} ;

	m->addAction( new QAction( QObject::tr( "Manage Favorites" ),m ) ) ;

	m->addSeparator() ;

	for( const auto& it : utility::readFavorites() ){

		m->addAction( _add_action( it ) ) ;
	}
}

void utility::licenseInfo( QWidget * parent )
{
	QString license = QString( "%1\n\n\
This program is free software: you can redistribute it and/or modify \
it under the terms of the GNU General Public License as published by \
the Free Software Foundation, either version 2 of the License, or \
( at your option ) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\
but WITHOUT ANY WARRANTY; without even the implied warranty of \
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the \
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>." ).arg( VERSION_STRING ) ;

	DialogMsg m( parent ) ;
	m.ShowUIInfo( QObject::tr( "about cryfs-gui" ),license ) ;
}

static utility::array_t _default_dimensions( const char * defaults )
{
	auto l = QString( defaults ).split( ' ' ) ;

	utility::array_t e ;

	auto f = e.data() ;

	auto j = l.size() ;

	for( int i = 0 ; i < j ; i++ ){

		*( f + i ) = l.at( i ).toInt() ;
	}

	return e ;
}

static utility::array_t _dimensions( const QString& path,const char * defaults,int size )
{
	QFile f( path ) ;

	if( !f.exists() ){

		if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ){

			f.write( defaults ) ;

			f.close() ;
		}else{
			qDebug() << "failed to open config file" ;
			return _default_dimensions( defaults ) ;
		}
	}

	if( f.open( QIODevice::ReadOnly ) ){

		auto l = utility::split( f.readAll(),' ' ) ;

		utility::array_t p ;

		if( l.size() != size || size > int( p.size() ) ){

			qDebug() << "failed to parse config file" ;
			return _default_dimensions( defaults ) ;
		}

		auto f = p.data() ;

		auto j = l.size() ;

		for( int i = 0 ; i < j ; i++ ){

			bool ok ;

			int e = l.at( i ).toInt( &ok ) ;

			if( ok ){

				*( f + i ) = e ;
			}else{
				qDebug() << "failed to parse config file option" ;
				return _default_dimensions( defaults ) ;
			}
		}

		return p ;
	}else{
		qDebug() << "failed to open config file" ;
		return _default_dimensions( defaults ) ;
	}
}

utility::array_t utility::getWindowDimensions( const QString& application )
{
	auto path = utility::homePath() + "/.cryfs-gui/" + application + "-gui-ui-options" ;

	return _dimensions( path,"205 149 861 466 384 320 121 0 0",9 ) ;
}

void utility::setWindowDimensions( const QString& application,const std::initializer_list<int>& e )
{
	auto path = utility::homePath() + "/.cryfs-gui/" + application + "-gui-ui-options" ;

	QFile f( path ) ;

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ){

		for( const auto& it : e ){

			f.write( QString( QString::number( it ) + " " ).toLatin1() ) ;
		}
	}
}

QFont utility::getFont( QWidget * widget )
{
	QString fontPath = utility::homePath() + "/.cryfs-gui/font" ;

	QFile x( fontPath ) ;

	if( x.open( QIODevice::ReadOnly ) ){

		auto l = utility::split( x.readAll() ) ;

		if( l.size() >= 4 ){

			QFont F ;

			const QString& fontFamily = l.at( 0 ) ;
			const QString& fontSize   = l.at( 1 ) ;
			const QString& fontStyle  = l.at( 2 ) ;
			const QString& fontWeight = l.at( 3 ) ;

			F.setFamily( fontFamily ) ;

			F.setPointSize( fontSize.toInt() ) ;

			if( fontStyle == "normal" ){

				F.setStyle( QFont::StyleNormal ) ;

			}else if( fontStyle == "italic" ){

				F.setStyle( QFont::StyleItalic ) ;
			}else{
				F.setStyle( QFont::StyleOblique ) ;
			}

			if( fontWeight == "normal" ){

				F.setWeight( QFont::Normal ) ;
			}else{
				F.setWeight( QFont::Bold ) ;
			}

			return F ;
		}else{
			return widget->font() ;
		}
	}else{
		return widget->font() ;
	}
}

void utility::saveFont( const QFont& Font )
{
	QFile f( utility::homePath() + "/.cryfs-gui/font" ) ;

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ){

		auto s = QString( "%1\n%2\n" ).arg( Font.family(),QString::number( Font.pointSize() ) ) ;

		if( Font.style() == QFont::StyleNormal ){

			s = s + "normal\n" ;

		}else if( Font.style() == QFont::StyleItalic ){

			s = s + "italic\n" ;
		}else{
			s = s + "oblique\n" ;
		}

		if( Font.weight() == QFont::Normal ){

			s = s + "normal\n" ;
		}else{
			s = s + "bold" ;
		}

		f.write( s.toLatin1() ) ;
	}
}

int utility::pluginKey( QDialog * w,QString * key,const QString& p )
{
	plugins::plugin pluginType ;
	QString pluginString ;
	QVector<QString> exe ;

	if( p == "hmac" ){

		pluginType   = plugins::plugin::hmac_key ;
		pluginString = QObject::tr( "hmac plugin.\n\nThis plugin generates a key using below formular:\n\nkey = hmac(sha256,passphrase,keyfile contents)" ) ;

	}else if( p == "keykeyfile" ){

		pluginType   = plugins::plugin::keyKeyFile ;
		pluginString = QObject::tr( "keykeyfile plugin.\n\nThis plugin generates a key using below formular:\n\nkey = passphrase + keyfile contents" ) ;

	}else if( p == "gpg" ){

		pluginType   = plugins::plugin::gpg ;
		pluginString = QObject::tr( "gpg plugin.\n\nThis plugin retrives a key locked in a gpg file with a symmetric key" ) ;

		if( utility::pathExists( "/usr/bin/gpg" ) ){

			exe.append( "/usr/bin/gpg" ) ;

		}else if( utility::pathExists( "/usr/local/bin/gpg" ) ){

			exe.append( "/usr/local/bin/gpg" ) ;

		}else if( utility::pathExists( "/usr/sbin/gpg" ) ){

			exe.append( "/usr/sbin/gpg" ) ;

		}else{

			DialogMsg msg( w ) ;

			msg.ShowUIOK( QObject::tr( "ERROR" ),QObject::tr( "Could not find \"gpg\" executable in \"/usr/local/bin\",\"/usr/bin\" and \"/usr/sbin\"" ) ) ;

			return 1 ;
		}

	}else{
		return 1 ;
	}

	QEventLoop l ;

	plugin::instance( w,pluginType,[ & ]( const QString& e ){

		*key = e ;

		if( e.isEmpty() ){

			l.exit( 1 ) ;
		}else{
			l.exit( 0 ) ;
		}

	},pluginString,exe ) ;

	return l.exec() ;
}

void utility::trayProperty( QSystemTrayIcon * trayIcon,bool zuluCrypt )
{
	Q_UNUSED( zuluCrypt ) ;

	QFile f( utility::homePath() + "/.cryfs-gui/tray" ) ;

	f.open( QIODevice::ReadOnly ) ;

	char c ;

	f.read( &c,1 ) ;

	f.close() ;

	f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ;

	if( c == '1' ){

		f.write( "0" ) ;
		trayIcon->hide() ;
	}else{
		f.write( "1" ) ;
		trayIcon->show() ;
	}
}

static void _set_checked( QMenu * m,const QString& e )
{
	for( auto& it : m->actions() ){

		QString p = it->text() ;

		p.remove( "&" ) ;

		it->setChecked( e == p ) ;
	}
}

void utility::setLocalizationLanguage( bool translate,QWidget * w,QAction * ac,const char * app )
{
	auto r = utility::localizationLanguage( app ).toLatin1() ;

	auto e = utility::localizationLanguagePath( app ) ;

	if( translate ){

		auto translator = new QTranslator( w ) ;

		if( r == "en_US" ){
			/*
			 * english_US language,its the default and hence dont load anything
			 */
		}else{
			translator->load( r.constData(),e ) ;
			QCoreApplication::installTranslator( translator ) ;
		}

	}else{
		auto m = new QMenu( w ) ;

		m->setFont( w->font() ) ;

		w->connect( m,SIGNAL( triggered( QAction * ) ),w,SLOT( languageMenu( QAction * ) ) ) ;

		QDir d( e ) ;

		m->addAction( "en_US" )->setCheckable( true ) ;

		auto t = d.entryList() ;

		if( !t.isEmpty() ){

			t.removeOne( "." ) ;
			t.removeOne( ".." ) ;

			for( auto& it : t ){

				m->addAction( it.remove( ".qm" ) )->setCheckable( true ) ;
			}
		}

		if( ac ){

			ac->setMenu( m ) ;
		}

		_set_checked( m,r ) ;
	}
}

void utility::languageMenu( QWidget * w,QMenu * m,QAction * ac,const char * app )
{
	Q_UNUSED( m ) ;

	auto e = ac->text() ;

	e.remove( "&" ) ;

	utility::setLocalizationLanguage( app,e ) ;

	DialogMsg msg( w ) ;

	msg.ShowUIOK( QObject::tr( "INFO" ),QObject::tr( "Translation will be done the next time you restart." ) ) ;
}

QStringList utility::directoryList( const QString& e )
{
	QDir d( e ) ;

	auto l = d.entryList() ;

	l.removeOne( "." ) ;
	l.removeOne( ".." ) ;

	return l ;
}

QIcon utility::getIcon( const QString& application )
{
	QIcon icon( INSTALL_PREFIX "/share/icons/hicolor/48x48/apps/" + application + ".png" ) ;

	return QIcon::fromTheme( application,icon ) ;
}

QString utility::homePath()
{
	return QDir::homePath() ;
}

bool utility::pathIsReadable( const QString& path )
{
	utility::fileHandle h ;

	return h.open( path ) ;
}

bool utility::pathExists( const QString& path )
{
	return QFile::exists( path ) ;
}

QStringList utility::split( const QString& e,char token )
{
	return e.split( token,QString::SkipEmptyParts ) ;
}

void utility::createPlugInMenu( QMenu * menu,const QString& a,const QString& b,const QString& c,bool addPlugIns )
{
	QStringList l ;
	QStringList e ;

	l.append( a ) ;

	if( LxQt::Wallet::backEndIsSupported( LxQt::Wallet::secretServiceBackEnd ) ){

		l.append( b ) ;
	}
	if( LxQt::Wallet::backEndIsSupported( LxQt::Wallet::kwalletBackEnd ) ){

		l.append( c ) ;
	}

	if( addPlugIns ){

	}

	menu->clear() ;

	auto _add_actions = [ menu ]( const QStringList& r ){

		for( const auto& it : r ){

			menu->addAction( it ) ;
		}
	} ;

	_add_actions( l ) ;
	_add_actions( e ) ;
}

static QString _language_path( const QString& program )
{
	return utility::homePath() + "/.cryfs-gui/" + program + ".lang" ;
}

QString utility::localizationLanguage( const QString& program )
{
	QFile f( _language_path( program ) ) ;

	if( f.open( QIODevice::ReadOnly ) ){

		QString e = f.readAll() ;

		e.remove( "\n" ) ;

		return e ;
	}else{
		return "en_US" ;
	}
}

void utility::setLocalizationLanguage( const QString& program,const QString& language )
{
	QFile f( _language_path( program ) ) ;

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ){

		f.write( language.toLatin1() ) ;
	}
}

QString utility::localizationLanguagePath( const QString& program )
{
	return QString( TRANSLATION_PATH ) + program ;
}


QString utility::walletName()
{
	return "cryfs-gui" ;
}

QString utility::applicationName()
{
	return "cryfs-gui" ;
}

QString utility::mountPath( const QString& path )
{
	return utility::homePath() + "/" + path ;
}

QString utility::mountPathPostFix( const QString& path )
{
	if( path.isEmpty() ){

		return path ;
	}else{
		auto _usable_mount_point = []( const QString& e ){

			return !utility::pathExists( e ) ;
		} ;

		auto e = utility::mountPath( path ) ;

		if( _usable_mount_point( e ) ){

			return path ;
		}else{
			QString z ;

			for( int i = 1 ; i < 1000 ; i++ ){

				z = QString::number( i ) ;

				if( _usable_mount_point( QString( "%1_%2" ).arg( e,z ) ) ){

					return QString( "%1_%2" ).arg( path,z ) ;
				}
			}

			return path ;
		}
	}
}

bool utility::setOpenVolumeReadOnly( QWidget * parent,bool checked,const QString& app )
{
	return readOnlyWarning::showWarning( parent,checked,app ) ;
}

bool utility::getOpenVolumeReadOnlyOption( const QString& app )
{
	return readOnlyWarning::getOpenVolumeReadOnlyOption( app ) ;
}
