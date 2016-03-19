#pragma once

#include <cstdint>
#include <string>
#include <list>

class CSteamAPIContext;
class IAddonDownloadNotification;
struct RemoteStorageGetPublishedFileDetailsResult_t;

namespace IAddonSystem
{

struct Information
{
	std::string title;
	std::string file;
	uint32_t pubdate;
	uint32_t models;
	uint64_t wsid;
	uint64_t creator;
	uint64_t placeholder7;
	uint32_t placeholder8;
	uint64_t placeholder9;
	bool placeholder10;
	bool downloaded;
};

}

namespace Addon
{

namespace Job
{

class Base;

}

class FileSystem
{
public:
	virtual void Clear( ) = 0;
	virtual void Refresh( ) = 0;
	virtual void MountFile( const std::string & ) = 0;
	virtual bool ShouldMount( const std::string & ) = 0;
	virtual bool ShouldMount( uint64_t ) = 0;
	virtual void SetShouldMount( const std::string &, bool ) = 0;
	virtual void Save( ) = 0;
	virtual const std::list<IAddonSystem::Information> &GetList( ) const = 0;
	virtual void ScanForSubscriptions( CSteamAPIContext *, const char * ) = 0;
	virtual void Think( ) = 0;
	virtual void SetDownloadNotify( IAddonDownloadNotification * ) = 0;
	virtual bool IsSubscribed( uint64_t ) = 0;
	virtual void FindFileOwner( const std::string & ) = 0;
	virtual void AddFile( const IAddonSystem::Information & ) = 0;
	virtual void ClearAllGMAs( ) = 0;
	virtual void Shutdown( ) = 0;
	virtual void AddFile( const RemoteStorageGetPublishedFileDetailsResult_t & ) = 0;
	virtual void AddJob( Job::Base * ) = 0;
	virtual void Notify( ) = 0;
	virtual bool HasChanges( ) = 0;
	virtual void MarkChanged( ) = 0;
	virtual void AddonDownloaded( IAddonSystem::Information & ) = 0;
	virtual void Load( ) = 0;
};

}
