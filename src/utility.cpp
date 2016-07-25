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
#include <QTimer>
#include <QEventLoop>

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

#include "lxqt_wallet.h"

#include "dialogmsg.h"
#include "plugin.h"

#include "version.h"

static int _help()
{
	std::cout << "\n" << VERSION_STRING << std::endl ;

	QString helpMsg = QObject::tr( "\n\
options:\n\
	-d   Path to where a volume to be auto unlocked/mounted is located.\n\
	-m   Tool to use to open a default file manager(default tool is xdg-open).\n\
	-e   Start the application without showing the GUI.\n\
	-b   A name of a backend to retrieve a password from when a volume is open from CLI.\n\
	     Supported backends are: \"internal\",\"kwallet\" and \"gnomewallet.\n\
	     The first one is always present but the rest are compile time dependencies.\n\
	-k   When opening a volume from CLI,a value of \"rw\" will open the volume in read\\write\n\
	     mode and a value of \"ro\" will open the volume in read only mode.\n\
	-z   Full path of the mount point to be used when the volume is opened from CLI.\n\
	     This option is optional.\n\
        -c   Set Cryfs Configuration File Path when a volume is opened from CLI.\n\
	-i   Set inactivity timeout(in minutes) to dismount the volume when mounted from CLI.\n\n" ) ;

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

static bool _execute_process( const QString& m,const QString& exe,const QString& env )
{
	Q_UNUSED( env ) ;

	if( exe.startsWith( "/" ) && utility::pathExists( exe ) ){

		auto e = m ;

		e.replace( "\"","\"\"\"" ) ;

		return utility::Task( exe + " \"" + e + "\"" ).success() ;
	}else{
		return false ;
	}
}

::Task::future<bool>& utility::openPath( const QString& path,const QString& opener,const QString& env )
{
	return ::Task::run<bool>( [ env,path,opener ](){

		return _execute_process( path,opener,env ) == false ;
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

::Task::future<QString>& utility::getKeyFromWallet( LXQt::Wallet::Wallet * wallet,const QString& volumeID )
{
	return ::Task::run<QString>( [ wallet,volumeID ](){

		return wallet->readValue( volumeID ) ;
	} ) ;
}

utility::wallet utility::getKeyFromWallet( QWidget * widget,
					   LXQt::Wallet::BackEnd storage,
					   const QString& keyID,const QString& pwd )
{
	utility::wallet w{ false,false,"","" } ;

	class Wallet
	{
	public:
		Wallet( LXQt::Wallet::BackEnd storage ) :
			m_wallet( LXQt::Wallet::getWalletBackend( storage ) )
		{
		}
		LXQt::Wallet::Wallet * operator->()
		{
			return m_wallet ;
		}
		LXQt::Wallet::Wallet * operator*()
		{
			return this->operator->() ;
		}
		~Wallet()
		{
			m_wallet->deleteLater() ;
		}
	private:
		LXQt::Wallet::Wallet * m_wallet ;
	} wallet( storage ) ;

	if( storage == LXQt::Wallet::BackEnd::kwallet || storage == LXQt::Wallet::BackEnd::libsecret ){

		if( storage == LXQt::Wallet::BackEnd::kwallet ){

			w.opened = wallet->open( "default",utility::applicationName() ) ;
		}else{
			w.opened = wallet->open( utility::walletName(),utility::applicationName() ) ;
		}

		if( w.opened ){

			w.key = utility::getKeyFromWallet( *wallet,keyID ).await() ;
		}

		return w ;

	}else if( storage == LXQt::Wallet::BackEnd::internal ){

		auto walletName = utility::walletName() ;
		auto appName    = utility::applicationName() ;

		if( LXQt::Wallet::walletExists( storage,walletName,appName ) ){

			wallet->setImage( QIcon( ":/cryfs-gui" ) ) ;

			w.opened = wallet->open( walletName,appName,widget,pwd ) ;

			if( w.opened ){

				w.key = utility::getKeyFromWallet( *wallet,keyID ).await() ;
				w.password = wallet->qObject()->objectName() ;
				w.notConfigured = false ;
			}

			return w ;
		}else{
			w.notConfigured = true ;
			return w ;
		}

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

QString utility::executableFullPath( const QString& e )
{
	QString exe ;

	for( const auto& it : { "/usr/local/bin/","/usr/local/sbin/","/usr/bin/","/usr/sbin/","/bin/","/sbin/" } ){

		exe = it + e ;

		if( utility::pathExists( exe ) ){

			return exe ;
		}
	}

	return QString() ;
}

QString utility::cmdArgumentValue( const QStringList& l,const QString& arg,const QString& defaulT )
{
	int j = l.size() ;

	auto _absolute_exe_path = []( const QString& exe ){

		auto e = utility::executableFullPath( exe ) ;

		if( e.isEmpty() ){

			return exe ;
		}else{
			return e ;
		}
	} ;

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

		QFile f( utility::homeConfigPath( "favorites" ) ) ;

		f.open( QIODevice::WriteOnly | QIODevice::Append ) ;

		f.write( fav.toLatin1() ) ;
	}
}

QStringList utility::readFavorites()
{
	QFile f( utility::homeConfigPath( "favorites" ) ) ;

	if( f.open( QIODevice::ReadOnly ) ){

		QStringList l ;

		for( const auto& it : utility::split( f.readAll() ) ){

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

	QFile f( utility::homeConfigPath( "favorites" ) ) ;

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

		return ac ;
	} ;

	m->addAction( new QAction( QObject::tr( "Manage Favorites" ),m ) ) ;
	m->addAction( new QAction( QObject::tr( "Mount All" ),m ) ) ;

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

static QString _windowDimensionsConfigPath()
{
	return utility::homeConfigPath( "cryfs-gui.dimensions" ) ;
}

utility::array_t utility::getWindowDimensions()
{
	auto path = _windowDimensionsConfigPath() ;

	return _dimensions( path,"205 149 861 466 326 320 101 76",8 ) ;
}

void utility::setWindowDimensions( const std::initializer_list<int>& e )
{
	QFile f( _windowDimensionsConfigPath() ) ;

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ){

		for( const auto& it : e ){

			f.write( QString( QString::number( it ) + " " ).toLatin1() ) ;
		}
	}
}

QFont utility::getFont( QWidget * widget )
{
	QString fontPath = utility::homeConfigPath( "font" ) ;

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
	QFile f( utility::homeConfigPath( "font" ) ) ;

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

void utility::trayProperty( QSystemTrayIcon * trayIcon,bool e )
{
	Q_UNUSED( e ) ;

	QFile f( utility::homeConfigPath( "tray" ) ) ;

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

class translator
{
public:
	void set( const QByteArray& e )
	{
		QCoreApplication::installTranslator( [ & ](){

			if( m_translator ){

				QCoreApplication::removeTranslator( m_translator ) ;

				delete m_translator ;
			}

			m_translator = new QTranslator() ;

			m_translator->load( e.constData(),utility::localizationLanguagePath() ) ;

			return m_translator ;
		}() ) ;
	}
	~translator()
	{
		//QCoreApplication::removeTranslator( m_translator ) ;
		delete m_translator ;
	}

private:
	QTranslator * m_translator = nullptr ;
} static _translator ;

static void _selectOption( QMenu * m,const QString& opt )
{
	utility::selectMenuOption s( m,false ) ;
	s.selectOption( opt ) ;
}

void utility::setLocalizationLanguage( bool translate,QMenu * m )
{
	auto r = utility::localizationLanguage().toLatin1() ;

	if( translate ){

		_translator.set( r ) ;
	}else{
		QDir d( utility::localizationLanguagePath() ) ;

		auto t = d.entryList() ;

		t.removeOne( "." ) ;
		t.removeOne( ".." ) ;

		for( auto& it : t ){

			m->addAction( it.remove( ".qm" ) )->setCheckable( true ) ;
		}

		_selectOption( m,r ) ;
	}
}

void utility::languageMenu( QWidget * w,QMenu * m,QAction * ac )
{
	Q_UNUSED( w ) ;

	auto e = ac->text() ;

	e.remove( "&" ) ;

	utility::setLocalizationLanguage( e ) ;

	utility::setLocalizationLanguage( true,m ) ;

	_selectOption( m,e ) ;
}

static QString _language_path()
{
	return utility::homeConfigPath( "cryfs-gui.lang" ) ;
}

QString utility::localizationLanguage()
{
	QFile f( _language_path() ) ;

	if( f.open( QIODevice::ReadOnly ) ){

		QString e = f.readAll() ;

		e.remove( "\n" ) ;

		return e ;
	}else{
		return "en_US" ;
	}
}

void utility::setLocalizationLanguage( const QString& language )
{
	QFile f( _language_path() ) ;

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ){

		f.write( language.toLatin1() ) ;
	}
}

QString utility::localizationLanguagePath()
{
	return QString( TRANSLATION_PATH ) ;
}

QStringList utility::directoryList( const QString& e )
{
	QDir d( e ) ;

	auto l = d.entryList() ;

	l.removeOne( "." ) ;
	l.removeOne( ".." ) ;

	return l ;
}

QIcon utility::getIcon()
{
	QIcon icon( INSTALL_PREFIX "/share/icons/hicolor/48x48/apps/cryfs-gui.png" ) ;

	return QIcon::fromTheme( "cryfs-gui",icon ) ;
}

QString utility::homeConfigPath( const QString& e )
{
	return utility::homePath() + "/.cryfs-gui/" + e ;
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

void utility::createPlugInMenu( QMenu * menu,const QString& a,const QString& b,const QString& c )
{
	QStringList l ;
	QStringList e ;

	l.append( a ) ;

	if( LXQt::Wallet::backEndIsSupported( LXQt::Wallet::BackEnd::libsecret ) ){

		l.append( b ) ;
	}
	if( LXQt::Wallet::backEndIsSupported( LXQt::Wallet::BackEnd::kwallet ) ){

		l.append( c ) ;
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

QString utility::walletName()
{
	return "cryfs-gui" ;
}

QString utility::applicationName()
{
	return "cryfs-gui" ;
}

static QString _mountPath = QString() ;

QString utility::mountPath( const QString& path )
{
	if( _mountPath.isEmpty() ){

		QFile f( utility::homeConfigPath( "mountPrefix" ) );

		if( f.open( QIODevice::ReadOnly ) ){

			_mountPath = f.readAll() ;

			_mountPath.remove( '\n' ) ;

			if( _mountPath.isEmpty() ){

				_mountPath = utility::homePath() + "/.cryfs-gui.tmp" ;
			}
		}else{
			_mountPath = utility::homePath() + "/.cryfs-gui.tmp" ;
		}
	}

	return _mountPath + "/" + path ;
}

QString utility::mountPathPostFix( const QString& path )
{
	return utility::mountPathPostFix( utility::mountPath( path ),path ) ;
}

QString utility::mountPathPostFix( const QString& e,const QString& path )
{
	if( path.isEmpty() ){

		return path ;
	}else{
		auto _usable_mount_point = []( const QString& e ){

			return !utility::pathExists( e ) ;
		} ;

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

bool utility::setOpenVolumeReadOnly( QWidget * parent,bool checked )
{
	return readOnlyWarning::showWarning( parent,checked ) ;
}

bool utility::getOpenVolumeReadOnlyOption()
{
	return readOnlyWarning::getOpenVolumeReadOnlyOption() ;
}

static QString _reUseMountPath()
{
	return utility::homeConfigPath( "reUseMountPoint.option" ) ;
}

bool utility::reUseMountPoint()
{
	return utility::pathExists( _reUseMountPath() ) ;
}

void utility::reUseMountPoint( bool e )
{
	if( e ){

		QFile f( _reUseMountPath() ) ;

		f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ;
	}else{
		QFile::remove( _reUseMountPath() ) ;
	}
}
