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

#pragma once
#include "3DMath.h"
#include "DecoderManager.h"
#include "IJob.h"
#include <string>



namespace BohgeEngine
{
	class IFile;
	class Decoder : public IJob
	{
		friend class DecoderManager;
	public:
		enum Format
		{
			DF_MONO_8 = 0,
			DF_STEREO_8,
			DF_MONO_16,
			DF_STEREO_16,
		};
	protected:
		enum SoundType
		{
			ST_OGG		= 0,
			ST_MP3		= 1,
			ST_WAV		= 2,
		};
	public:
		class BufferChunk
		{
		private:
			char*			m_pBuffer;//�����ڴ��
			uint			m_nSize;//���ݿ��С
			uint			m_nNextPosition;//��һ�����ݵ�����
			bool			m_isDone;
		public:
			BufferChunk( char* buffer, uint size, uint next, bool done )
				:m_pBuffer( buffer ),
				m_nSize( size ),
				m_nNextPosition( next ),
				m_isDone( done )
			{}
		public:
			BOHGE_FORCEINLINE bool isDone( ) const
			{
				return m_isDone;
			}
			BOHGE_FORCEINLINE const char* GetBuffer( ) const
			{
				return m_pBuffer;
			}
			BOHGE_FORCEINLINE uint GetSize( ) const
			{
				return m_nSize;
			}
			BOHGE_FORCEINLINE uint GetNextIndex( ) const
			{
				return m_nNextPosition;
			}
		};
	private:
		typedef std::vector<char*> BufferVector;
	private:
		int						m_nFrequency;//Ƶ��
		int						m_nChannels;//����
		int						m_nSourceTotalSize;//����pcm���ܴ�С
		double					m_dTime;
		Format					m_nFormat;//��ʽ
		uint					m_nLoadedBufferIndex;//�Ѿ����ع�������λ��
		std::string				m_FilePath;
		SoundType				m_eDecoderTypes;
		bool					m_isLoaded;
		bool			m_isActived;//��Դ�Ƿ���Խ��н��룬��ȡ�Ȳ�����������ReleaseDecoder������Ϊfalse
		bool					m_isRequested;//�Ƿ��Ѿ��ڶ����в����������
		bool					m_isDecoding;//�Ƿ��ڽ���֮��
		IFile*					m_pFileStream;//io
		BufferVector			m_BufferVector;//��Դ��
	protected:
		Decoder( SoundType type );
	public:
		virtual ~Decoder(void);
	private:
		virtual void _DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time ) = 0;
		virtual void _DoReleaseDecoder() = 0;//�ͷ���Դ
		virtual uint _DoDecodeAsyn( uint form, uint to ) = 0;//�첽����,���ؼ��ص�����
	private:
		bool _PrepareDecode();//׼���������ݶ�
	public:
		void LoadResource( const std::string& path );
		void ReleaseDecoder( );//�ͷ�io��buffer
		BufferChunk GetBufferChunk( int index );//�õ���������,���Զ�������һ�μ��أ����getֻ��һ�ε���Ч��
	public:
		virtual void DoJob(SmartPtr<IJob>& self);//�̳���IAsynJob
	private:
		BOHGE_FORCEINLINE uint _GetEndPosition( uint form )
		{
			return form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE > m_nSourceTotalSize ? m_nSourceTotalSize : form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		}
		BOHGE_FORCEINLINE void _Deactive()//�ر�decoder��׼���ͷ�
		{
			m_isActived = false;
		}
	protected:
		BOHGE_FORCEINLINE IFile* _GetFileSteam()
		{
			return m_pFileStream;
		}
		BOHGE_FORCEINLINE char* _GetInternalBuffer()
		{
			return m_BufferVector[m_nLoadedBufferIndex];
		}
	public:
		BOHGE_FORCEINLINE int isChunkReady( uint index ) const //���ݶ��Ƿ�׼����
		{
			return index < m_nLoadedBufferIndex;
		}
		BOHGE_FORCEINLINE int GetFrequency() const
		{
			return m_nFrequency;
		}
		BOHGE_FORCEINLINE Format GetFormat() const
		{
			return m_nFormat;
		}
		BOHGE_FORCEINLINE int GetChannels() const
		{
			return m_nChannels;
		}
		BOHGE_FORCEINLINE int GetSourceSize() const
		{
			return m_nSourceTotalSize;
		}
		BOHGE_FORCEINLINE bool isActived() const
		{
			return m_isActived;
		}
		BOHGE_FORCEINLINE bool isRequested() const
		{
			return m_isRequested;
		}
		BOHGE_FORCEINLINE bool isDecoding() const
		{
			return m_isDecoding;
		}
		BOHGE_FORCEINLINE SoundType GetDecoderType() const
		{
			return m_eDecoderTypes;
		}
		BOHGE_FORCEINLINE const std::string& GetFilePath() const
		{
			return m_FilePath;
		}
	};
}
