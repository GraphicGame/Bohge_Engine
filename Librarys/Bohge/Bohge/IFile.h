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



/////////////////////////////////
//			�ļ�ϵͳ		   //
/////////////////////////////////
#pragma once
#include "Predefine.h"
#include "VariableType.h"
#include <string>
#include <vector>


namespace BohgeEngine
{
	//��������
	class IFile
	{
		friend class IOSystemBase;
	public:
		enum ActionType
		{
			AT_NONE = 0, //û�г�ʼ����
			AT_READ,
			AT_WRITE,
		};
	private:
		enum Constant
		{
			FC_FILEBUFFER = 1024*32,//32kb�����buffer
		};
	private:
		std::string				m_FileUrl;
		ActionType				m_ActionType;
		byte*					m_pDatas;
		uint					m_BufferSize;//����Ĵ�С�����ļ�С��FC_FILEBUFFER����Ҫ���㻺���С
		uint					m_FileSize;
		uint					m_PosIndex;
		uint					m_ReadedBufferSize;//�Ѿ����ص�buffer��С
		bool					m_isOpen;
	protected:
		IFile( const std::string& url );
		virtual ~IFile(void);
	protected:
		virtual bool _DoOpenFile( ActionType at ) = 0;
		virtual bool _DoCloseFile() = 0;
		virtual int _DoSeekFile( int to, int whence ) = 0;
		virtual int _DoTell() = 0;
		virtual int _DoReadFile( void* data, uint bitesize ) = 0; //ʵ�ʵĶ�ȡ����
		virtual int _DoWriteFile( const void* data, uint bitesize ) = 0; //ʵ�ʵ�д�뺯��
	public:
		bool OpenFile( ActionType at );
		bool CloseFile();
		int Seek( int to, int whence );
		int Tell();
		int ReadFile( void* data, uint bitesize );
		int WriteFile( const void* data, uint bitesize );
	protected:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint _GetFileSize( )
		{
			return m_FileSize;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void _SetFileSize( uint size )
		{
			m_FileSize = size;
		}
	public:
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const std::string& GetFilePath()
		{
			return m_FileUrl;
		}
		//-------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint GetSize()
		{
			return m_FileSize;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int ReadFile( T* ptr )
		{
			ReadFile( ptr, sizeof(T) );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T> BOHGE_FORCEINLINE int WriteFile( T* ptr )
		{
			WriteFile( ptr, sizeof(T) );
		}
	};
}