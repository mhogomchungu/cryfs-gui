/*
 *
 *  Copyright (c) 2015
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

#include "checkforupdates.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QWidget>
#include <QFile>

#include "utility.h"
#include "dialogmsg.h"

#include "version.h"

static QString _tr( const QString& n,const QString& a,const QStringList& l )
{
	auto e = QObject::tr( "%1\"%2\" Installed Version Is : %3.\nLatest Version Is : %4." ) ;
	return e.arg( n,a,l.at( 0 ),l.at( 1 ) ) ;
}

static void _show( QObject * obj,bool autocheck,QWidget * w,QString l,const QStringList& e )
{
	DialogMsg msg( w ) ;

	if( l.isEmpty() ){

		auto e = QObject::tr( "Failed To Check For Update." ) ;
		msg.ShowUIOK( QObject::tr( "ERROR" ),e ) ;
	}else{
		l.replace( "\n","" ) ;

		if( autocheck ){

			auto _mismatch = [ & ]{

				if( e.size() == 2 ){

					return e.at( 0 ) != e.at( 1 ) ;
				}else{
					return false ;
				}
			} ;

			if( ( l != "Not Found" && l != THIS_VERSION ) || _mismatch() ){

				l = _tr( "\n","cryfs-gui",{ THIS_VERSION,l } ) ;

				if( !e.isEmpty() ){

					l += _tr( "\n\n","cryfs",e ) ;
				}

				msg.ShowUIOK( QObject::tr( "Update Available" ),l + "\n" ) ;
			}
		}else{
			if( l != "Not Found" ){

				l = _tr( "\n","cryfs-gui",{ THIS_VERSION,l } ) ;

				if( !e.isEmpty() ){

					l += _tr( "\n\n","cryfs",e ) ;
				}

				msg.ShowUIOK( QObject::tr( "Version Info" ),l + "\n" ) ;
			}else{
				auto e = QObject::tr( "Failed To Check For Update." ) ;

				msg.ShowUIOK( QObject::tr( "ERROR" ),e ) ;
			}
		}
	}

	obj->deleteLater() ;
}

#if QT_VERSION < QT_VERSION_CHECK( 5,0,0 )

void checkForUpdates::networkReply( QNetworkReply * p )
{
	_show( this,m_autocheck,m_widget,p->readAll(),{} ) ;
}

#else

#include <QJsonDocument>

void checkForUpdates::networkReply( QNetworkReply * p )
{
	if( m_firstTime ){

		m_data = p->readAll() ;

		this->getUpdate( false ) ;
	}else{
		setenv( "CRYFS_NO_UPDATE_CHECK","TRUE",1 ) ;
		setenv( "CRYFS_FRONTEND","noninteractive",1 ) ;

		auto e = utility::Task::run( "cryfs" ).await().output().split( ' ' ) ;

		if( e.size() >= 3 ){

			QJsonParseError error ;

			auto r = QJsonDocument::fromJson( p->readAll(),&error ) ;

			if( error.error == QJsonParseError::NoError ){

				auto m = r.toVariant().toMap() ;

				if( !m.isEmpty() ){

					m = m[ "version_info" ].toMap() ;
				}

				if( !m.isEmpty() ){

					auto d = m[ "current" ].toString() ;

					if( !d.isEmpty() ){

						auto f = QString( e.at( 2 ) ).split( '\n' ).first() ;

						return _show( this,m_autocheck,m_widget,m_data,{ f,d } ) ;
					}
				}
			}
		}

		_show( this,m_autocheck,m_widget,m_data,{} ) ;
	}
}

#endif

checkForUpdates::checkForUpdates( QWidget * widget,bool autocheck ) :
	m_widget( widget ),m_autocheck( autocheck )
{
	connect( &m_manager,SIGNAL( finished( QNetworkReply * ) ),
		 this,SLOT( networkReply( QNetworkReply * ) ) ) ;

	this->getUpdate( true ) ;
}

void checkForUpdates::getUpdate( bool e )
{
	m_firstTime = e ;

	auto url_0 = "https://raw.githubusercontent.com/mhogomchungu/cryfs-gui/master/version" ;
	auto url_1 = "https://www.cryfs.org/version_info.json" ;

	auto user_agent = "Mozilla/5.0 (X11; Linux x86_64; rv:46.0) Gecko/20100101 Firefox/46.0" ;

	m_manager.get( [ & ](){

		auto e = [ & ](){

			if( m_firstTime ){

				QNetworkRequest e( [ & ](){ QUrl u( url_0 ) ; return u ; }() ) ;

				e.setRawHeader( "Host","raw.githubusercontent.com" ) ;

				return e ;
			}else{
				QNetworkRequest e( [ & ](){ QUrl u( url_1 ) ; return u ; }() ) ;

				e.setRawHeader( "Host","www.cryfs.org" ) ;

				return e ;
			}
		}() ;

		e.setRawHeader( "User-Agent",user_agent ) ;
		e.setRawHeader( "Accept-Encoding","text/plain" ) ;

		return e ;
	}() ) ;
}

static QString _optionPath()
{
	return utility::homePath() + "/.cryfs-gui/autoCheckUpdates.cryfs-gui" ;
}

void checkForUpdates::instance( QWidget * widget,const QString& e )
{
	Q_UNUSED( e ) ;

	if( utility::pathExists( _optionPath() ) ){

		new checkForUpdates( widget,true ) ;
	}
}

void checkForUpdates::instance( QWidget * widget )
{
	new checkForUpdates( widget,false ) ;
}

bool checkForUpdates::autoCheck()
{
	return utility::pathExists( _optionPath() ) ;
}

void checkForUpdates::autoCheck( bool e )
{
	QFile f( _optionPath() ) ;

	if( e ){

		f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ;
	}else{
		f.remove() ;
	}
}

checkForUpdates::~checkForUpdates()
{
}
