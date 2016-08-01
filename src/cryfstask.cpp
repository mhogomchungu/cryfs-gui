/*
 *
 *  Copyright (c) 2014-2015
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

#include "cryfstask.h"

#include <QDir>
#include <QString>
#include <QDebug>
#include <QFile>

#include <iostream>

using cs = cryfsTask::status ;

static bool _create_folder( const QString& m )
{
	if( utility::pathExists( m ) ){

		return utility::reUseMountPoint() ;
	}else{
		QDir e ;
		return e.mkpath( m ) ;
	}
}

static QString _makePath( const QString& e )
{
	return utility::Task::makePath( e ) ;
}

bool cryfsTask::deleteMountFolder( const QString& m )
{
	if( utility::reUseMountPoint() ){

		return false ;
	}else{
		QDir e ;
		return e.rmdir( m ) ;
	}
}

Task::future< bool >& cryfsTask::encryptedFolderUnMount( const QString& m )
{
	return Task::run< bool >( [ m ](){

		auto cmd = "fusermount -u " + _makePath( m ) ;

		utility::Task::waitForOneSecond() ;

		for( int i = 0 ; i < 5 ; i++ ){

			if( utility::Task( cmd,10000 ).success() ){

				return true ;
			}else{
				utility::Task::waitForOneSecond() ;
			}
		}

		return false ;
	} ) ;
}

static cs _cmd( bool create,const QString& app,const cryfsTask::options& opt,
		const QString& password,const QString& configFilePath )
{
        auto _args = []( const QString& exe,const cryfsTask::options& opt,
			 const QString& type,const QString& configFilePath,bool create ){

		auto cipherFolder = _makePath( opt.cipherFolder ) ;

		auto mountPoint   = _makePath( opt.plainFolder ) ;

		auto mountOptions = [ & ](){

			if( !opt.mOpt.isEmpty() ){

				if( type == "cryfs" ){

					return QString( "--unmount-idle %1" ).arg( opt.mOpt ) ;

				}else if( type == "encfs" ){

					return QString( "--idle=%1" ).arg( opt.mOpt ) ;
				}
			}

			return QString() ;
		}() ;

		auto separator = [ & ](){

			if( type == "cryfs" ){

				return "--" ;

			}else if( type == "encfs" ){

				return "-S" ;
			}else{
				return "" ;
			}
		}() ;

                auto configPath = [ & ](){

			if( ( type == "cryfs" || type == "gocryptfs" || type == "securefs" )
					&& !configFilePath.isEmpty() ){

				return "--config " + _makePath( configFilePath ) ;
                        }else{
                                return QString() ;
                        }
                }() ;

		if( type == "gocryptfs" || type == "securefs" ){

			if( type == "gocryptfs" ){

				if( create ){

					return exe + " " + configPath + " --init " + cipherFolder ;
				}else{
					auto mode = [ & ](){

						if( opt.ro ){

							return "-ro" ;
						}else{
							return "" ;
						}
					}() ;

					auto e = "%1 %2 %3 %4 %5" ;

					return QString( e ).arg( exe,mode,configPath,cipherFolder,mountPoint ) ;
				}
			}else{
				if( create ){

					auto e = QString( "%1 c %2 %3" ) ;
					return e.arg( exe,configPath,cipherFolder ) ;
				}else{
					auto mode = [ & ](){

						if( opt.ro ){

							return "-o ro" ;
						}else{
							return "-o rw" ;
						}
					}() ;

					auto e = QString( "%1 m -b %2 %3 -o fsname=securefs@%4 -o subtype=securefs %5 %6" ) ;

					return e.arg( exe,configPath,mode,cipherFolder,cipherFolder,mountPoint ) ;
				}
			}
		}else{
			QString e = "%1 %2 %3 %4 %5 %6 -o fsname=%7@%8 -o subtype=%9" ;

			auto opts = e.arg( exe,cipherFolder,mountPoint,mountOptions,configPath,
					   separator,type,cipherFolder,type ) ;

			if( opt.ro ){

				return opts + " -o ro" ;
			}else{
				return opts + " -o rw" ;
			}
		}
	} ;

	auto exe = utility::executableFullPath( app ) ;

	if( exe.isEmpty() ){

		if( app == "cryfs" ){

			return cs::cryfsNotFound ;

		}else if( app == "encfs" ){

			return cs::encfsNotFound ;

		}else if( app == "securefs" ){

			return cs::securefsNotFound ;
		}else{
			return cs::gocryptfsNotFound ;
		}
	}else{
		auto e = utility::Task( _args( exe,opt,app,configFilePath,create ),20000,[](){

			QProcessEnvironment env ;

			env.insert( "CRYFS_NO_UPDATE_CHECK","TRUE" ) ;
			env.insert( "CRYFS_FRONTEND","noninteractive" ) ;

			return env ;

		}(),password.toLatin1() ) ;

		auto _taskOutput = [ & ](){

			return e.output() + "\n-----\n" + e.stdError() ;
		} ;

		if( e.finished() ){

			if( e.success() ){

				return cs::success ;
			}else{
				auto a = "Did you enter the correct password?" ;
				auto b = "password incorrect" ;
				auto c = "Password incorrect" ;
				auto d = "Invalid password" ;

				if( utility::containsAtleastOne( _taskOutput(),a,b,c,d ) ){

					std::cout << _taskOutput().constData() << std::endl ;

					if( app == "cryfs" ){

						return cs::cryfs ;

					}else if( app == "encfs" ){

						return cs::encfs ;						

					}else if( app == "securefs" ){

						return cs::securefs ;
					}else{
						return cs::gocryptfs ;
					}
				}
			}
		}

		std::cout << _taskOutput().constData() << std::endl ;

		return cs::backendFail ;
	}
}

static QString _configFilePath( const cryfsTask::options& opt )
{
	if( opt.configFilePath.startsWith( "/" ) || opt.configFilePath.isEmpty() ){

		return opt.configFilePath ;
	}else{
		return utility::homePath() + "/" + opt.configFilePath ;
	}
}

Task::future< cs >& cryfsTask::encryptedFolderMount( const options& opt,bool reUseMountPoint )
{
	return Task::run< cs >( [ opt,reUseMountPoint ](){

		auto _mount = [ reUseMountPoint ]( const QString& app,const options& opt,
				const QString& configFilePath ){

			if( _create_folder( opt.plainFolder ) || reUseMountPoint ){

				auto e = _cmd( false,app,opt,opt.key,configFilePath ) ;

				if( e == cs::success ){

					opt.openFolder( opt.plainFolder ) ;
				}else{
					cryfsTask::deleteMountFolder( opt.plainFolder ) ;
				}

				return e ;
			}else{
				return cs::failedToCreateMountPoint ;
			}
		} ;

                if( opt.configFilePath.isEmpty() ){

                        if( utility::pathExists( opt.cipherFolder + "/cryfs.config" ) ){

				return _mount( "cryfs",opt,QString() ) ;

			}else if( utility::pathExists( opt.cipherFolder + "/gocryptfs.conf" ) ){

				return _mount( "gocryptfs",opt,QString() ) ;

			}else if( utility::pathExists( opt.cipherFolder + "/.securefs.json" ) ){

				return _mount( "securefs",opt,QString() ) ;
			}else{
                                auto encfs6 = opt.cipherFolder + "/.encfs6.xml" ;
                                auto encfs5 = opt.cipherFolder + "/.encfs5" ;
                                auto encfs4 = opt.cipherFolder + "/.encfs4" ;

                                if( utility::atLeastOnePathExists( encfs6,encfs5,encfs4 ) ){

					return _mount( "encfs",opt,QString() ) ;
                                }else{
                                        return cs::unknown ;
                                }
                        }
                }else{
			auto e = _configFilePath( opt ) ;

			if( utility::pathExists( e ) ){

				if( e.endsWith( "gocryptfs.conf" ) ){

					return _mount( "gocryptfs",opt,e ) ;

				}else if( e.endsWith( "securefs.json" ) ){

					return _mount( "securefs",opt,e ) ;
				}else{
					return _mount( "cryfs",opt,e ) ;
				}
			}else{
				return cs::unknown ;
			}
                }
	} ) ;
}

Task::future< cs >& cryfsTask::encryptedFolderCreate( const options& opt )
{
	return Task::run< cs >( [ opt ](){

		if( _create_folder( opt.cipherFolder ) ){

			if( _create_folder( opt.plainFolder ) ){

				auto e = _cmd( true,opt.exe,opt,[ & ]()->QString{

					if( opt.exe == "cryfs" || opt.exe == "gocryptfs" ){

						return opt.key ;

					}else if( opt.exe == "securefs" ){

						return opt.key + "\n" + opt.key ;
					}else{
						return "p\n" + opt.key ;
					}

				}(),_configFilePath( opt ) ) ;

				if( e == cs::success ){

					if( opt.exe == "gocryptfs" || opt.exe == "securefs" ){

						e = cryfsTask::encryptedFolderMount( opt,true ).get() ;

						if( e != cs::success ){

							cryfsTask::deleteMountFolder( opt.plainFolder ) ;
						}
					}else{
						opt.openFolder( opt.plainFolder ) ;
					}
				}else{
					cryfsTask::deleteMountFolder( opt.plainFolder ) ;
					cryfsTask::deleteMountFolder( opt.cipherFolder ) ;
				}

				return e ;
			}else{
				cryfsTask::deleteMountFolder( opt.cipherFolder ) ;

				return cs::failedToCreateMountPoint ;
			}
		}else{
			return cs::failedToCreateMountPoint ;
		}
	} ) ;
}

Task::future< QVector< volumeInfo > >& cryfsTask::updateVolumeList()
{
	return Task::run< QVector< volumeInfo > >( [](){

		auto _hash = []( const QString& e ){

			/*
			 * jenkins one at a time hash function.
			 *
			 * https://en.wikipedia.org/wiki/Jenkins_hash_function
			 */

			uint32_t hash = 0 ;

			auto p = e.toLatin1() ;

			auto key = p.constData() ;

			auto l = p.size() ;

			for( decltype( l ) i = 0 ; i < l ; i++ ){

				hash += *( key + i ) ;

				hash += ( hash << 10 ) ;

				hash ^= ( hash >> 6 ) ;
			}

			hash += ( hash << 3 ) ;

			hash ^= ( hash >> 11 ) ;

			hash += ( hash << 15 ) ;

			return QString::number( hash ) ;
		} ;

		auto _dcd = []( QString path,bool set_offset ){

			path.replace( "\\012","\n" ) ;
			path.replace( "\\040"," " ) ;
			path.replace( "\\134","\\" ) ;
			path.replace( "\\011","\\t" ) ;

			if( set_offset ){

				return path.mid( path.indexOf( '@' ) + 1 ) ;
			}else{
				return path ;
			}
		} ;

		auto _fs = []( const QString& e ){

			return e.mid( 5 ) ;
		} ;

		auto _ro = []( const QStringList& l ){

			const auto& e = l.at( 5 ) ;

			if( e.startsWith( "ro," ) ){

				return "ro" ;
			}else{
				return "rw" ;
			}
		} ;

		QVector< volumeInfo > e ;

		for( const auto& it : utility::monitor_mountinfo::mountinfo() ){

			if( utility::containsAtleastOne( it," fuse.cryfs ",
							 " fuse.encfs ",
							 " fuse.gocryptfs ",
							 " fuse.securefs " ) ){

				const auto& k = utility::split( it,' ' ) ;

				const auto s = k.size() ;

				const auto& cf = k.at( s - 2 ) ;

				const auto& m = k.at( 4 ) ;

				const auto& fs = k.at( s - 3 ) ;

				if( utility::startsWithAtLeastOne( cf,"encfs@","cryfs@","securefs@" ) ){

					e.append( { _dcd( cf,true ),_dcd( m,false ),_fs( fs ),_ro( k ) } ) ;

				}else if( fs == "fuse.gocryptfs" ){

					e.append( { _dcd( cf,false ),_dcd( m,false ),_fs( fs ),_ro( k ) } ) ;
				}else{
					e.append( { _hash( m ),_dcd( m,false ),_fs( fs ),_ro( k ) } ) ;
				}
			}
		}

		return e ;
	} ) ;
}
