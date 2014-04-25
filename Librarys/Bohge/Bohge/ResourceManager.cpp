//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



/////////////////////////
//		��Դ��ȡ	   //
/////////////////////////
#include "ResourceManager.h"
#include "Model.h"
#include "Texture.h"
#include "Device.h"
#include "Engine.h"
#include "Log.h"
#include "TextureData.h"
#include "Utility.h"
#include "ISource.h"
#include "IResource.h"
#include "GreaterThread.h"
#include "ThreadMutex.h"


using namespace std;


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ResourceManager::ResourceManager()
	{
		m_pLoadingThread = NEW GreaterThread();
		m_pLoadingThread->SetPriority( Threading::TP_HIGH );
		m_pLoadingThread->Start();

		m_pMutex = NEW ThreadMutex();
	}
	//------------------------------------------------------------------------------------------------------
	ResourceManager::~ResourceManager()
	{
		m_pLoadingThread->Stop();
		SAFE_DELETE( m_pLoadingThread );
		SAFE_DELETE( m_pMutex );
	}
	//------------------------------------------------------------------------------------------------------
	const TextureData& ResourceManager::LoadTexture2D( Device::PixelFormat pf, const std::string& filename)
	{
		TextureReferenceMap::iterator it = m_TextureMap.find( Utility::HashCode(filename + static_cast<char>( pf+1 )) );
		if( it != m_TextureMap.end()  )
		{
			it->second.second++;
			return *(it->second.first);
		}
		TextureData* pdata = NEW TextureData();
		if( Engine::Instance().GetDevice()->LoadTextureFromFile( pf, *pdata, filename) )
		{
			m_TextureMap.insert( std::make_pair( Utility::HashCode(filename + static_cast<char>( pdata->m_eFormat+1 )), std::make_pair( pdata, 1 ) ) );
			return *pdata;
		}
		ASSERT(false);
		return *pdata;
	}
	//------------------------------------------------------------------------------------------------------
	const TextureData& ResourceManager::LoadTextureCube( Device::PixelFormat pf, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r )
	{
		std::string filename = f+b+u+d+l+r;
		TextureReferenceMap::iterator it = m_TextureMap.find( Utility::HashCode(filename + static_cast<char>( pf+1 )) );
		if( it != m_TextureMap.end()  )
		{
			it->second.second++;
			return *(it->second.first);
		}

		TextureData* pdata = NEW TextureData();
		if( Engine::Instance().GetDevice()->LoadCubeTextureFromFile( pf, *pdata, f, b, u, d, l, r ) )
		{
			m_TextureMap.insert( std::make_pair( Utility::HashCode(filename + static_cast<char>( pdata->m_eFormat+1 )), std::make_pair( pdata, 1 ) ) );
			return *pdata;
		}
		ASSERT(false);
		return *pdata;
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::ReleaseTexture(Device::PixelFormat pf, const std::string& name)
	{
		TextureReferenceMap::iterator it = m_TextureMap.find( Utility::HashCode(name + static_cast<char>( pf+1 )) );
		if( it != m_TextureMap.end()  )
		{
			it->second.second--;
			if ( 0 == it->second.second )
			{
				//DEBUGLOG("Release %s\n", name.c_str() );
				Engine::Instance().GetDevice()->DeleteTexture( it->second.first->m_hTextureHandle );
				SAFE_DELETE( it->second.first );
				m_TextureMap.erase( it++ );
			}
			return;
		}
		//��������Ҳ���
		ASSERT(false);
	}
	//------------------------------------------------------------------------------------------------------
	const ModelData& ResourceManager::LoadModel( const std::string& name )
	{
		ModelData* data;
		ModelsReferenceMap::iterator it = m_ModelMap.find( Utility::HashCode(name) );
		if ( it == m_ModelMap.end() )
		{
			data = NEW ModelData();
			data->LoadModel( name );
			m_ModelMap.insert( make_pair( Utility::HashCode(name), make_pair( data, 1 ) ) );
		}
		else
		{
			data = it->second.first;
		}
		return *data;
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::ReleaseModel( const std::string& name )
	{
		ModelsReferenceMap::iterator it = m_ModelMap.find( Utility::HashCode(name) );
		if( m_ModelMap.end() != it )
		{
			it->second.second--;
			if( 0 == it->second.second )//�ͷ�
			{
				SAFE_DELETE( (it->second.first) );
				m_ModelMap.erase( it );
			}
		}
	}



	//------------------------------------------------------------------------------------------------------
	void ResourceManager::OnSourceLoaded( SmartPtr<ISource>& source )
	{
		//���������첽�̼߳�����Ϻ󣬻�����������������push�������У��ȴ������߳���������Դ
		m_pMutex->Lock();
		m_LoadedSource.push( source );
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::LoadResource( SmartPtr<ISource>& source )
	{
		m_pMutex->Lock();//����Ҳ��Ҫ���������첽�߳̿��ܵ������push�µ���Դ����
		IResourcePairMap::iterator res = m_IResourcePairMap.find( source->GetHashCode() );
		if ( m_IResourcePairMap.end() != res )//�ҵ������Դ��ֱ��֪ͨ����,������ӵ��ȴ�������
		{
			if ( NULL != res->second->Resource )
			{
				res->second->Resource->Increment();//�������ü���
				Event<bool,IResource*>::FunctionType* ptr = source->GetListener();
				res->second->User->Connect( ptr );
				ptr->Invoker( res->second->Resource );//֪ͨ�ͻ���Դ�Ѿ�׼������
			}
			else//��Դ���ڼ�����
			{
				Event<bool,IResource*>::FunctionType* ptr = source->GetListener();
				res->second->User->Connect( ptr );
			}
		}
		else
		{
				//�����û�ҵ�������ӵ����ض�����
				Event<bool,IResource*>* eventPtr = NEW Event<bool,IResource*>();
				eventPtr->Connect( source->GetListener() );
				ResourcePair* pair = NEW ResourcePair();
				pair->User = eventPtr;
				m_IResourcePairMap.insert( make_pair( source->GetHashCode(), pair ) );
				m_pLoadingThread->PushJob( source );
		}
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::Update()
	{
		while( !m_LoadedSource.empty() )
		{
			m_pMutex->Lock();
			SmartPtr<ISource> ptr = m_LoadedSource.front();
			m_LoadedSource.pop();
			m_pMutex->Unlock();
			IResourcePairMap::iterator pair = m_IResourcePairMap.find( ptr->GetHashCode() );
			uint neederCount = pair->second->User->GetListenerCount();
			if ( 0 != neederCount )//�������������
			{
				IResource* res = ptr->DoMakeResource();//��������
				pair->second->Resource = res;
				pair->second->User->Multicast( res );//֪ͨ�ͻ�����Դ�Ѿ�׼�����
				res->Increment( neederCount );//������ü���
			}
			else//û������Ͱ׼����ˣ�ֱ��ɾ����Դ
			{
				SAFE_DELETE( pair->second->User );//ɾ���¼�
				SAFE_DELETE( pair->second );//ɾ����Դ��
				m_IResourcePairMap.erase( pair );//ɾ����Դ�����е�����
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	void ResourceManager::UnloadResource( SmartPtr<ISource>& source )
	{
		IResourcePairMap::iterator pair = m_IResourcePairMap.find( source->GetHashCode() );
		if ( NULL != pair->second->Resource )//�ҵ������Դ��ֱ�Ӽ������ü���
		{
			if( 0 == pair->second->Resource->Decrement() )//�������ü���
			{
				pair->second->Resource->ReleaseResource();//�ͷ���Դ
				SAFE_DELETE( pair->second->Resource );//ɾ����Դ
				SAFE_DELETE( pair->second->User );//ɾ���¼�
				SAFE_DELETE( pair->second );//ɾ����Դ��
				m_IResourcePairMap.erase( pair );//ɾ����Դ�����е�����
			}
		}
		else
		{
			pair->second->User->Remove( source->GetListener() );//����ڼ��ض����о�ȥ���¼�������
		}
	}

}