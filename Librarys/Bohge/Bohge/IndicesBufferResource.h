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
#include "IResource.h"
#include "BufferObjectStatus.h"




namespace BohgeEngine
{
	//����VBO
	class IndicesBufferResource : public IResource
	{
	private:
		static handle							s_CurrentHandle;
	private:
		BufferObjectStatus::IndicesType			m_eIndicesType;
		BufferObjectStatus::MemoryUseage		m_eMemoryUseage;
		handle									m_BufferHandle;
		uint									m_uIndiesCount;//��������Ŀ
		uint									m_DrawCount;
		byte									m_TypeSize;
	public:
		IndicesBufferResource(void);
		virtual ~IndicesBufferResource(void);
	private:
		virtual void _DoDeleteBuffer( handle& h ) = 0;//ɾ��
		virtual void _DoBindBuffer( handle h ) = 0;//��
		virtual handle _DoCreateBuffer(uint datasize, uint typesize, const void* dataptr, BufferObjectStatus::MemoryUseage use) = 0;//����
		virtual void _DoChangeData(handle h, uint offset, uint datasize, uint typesize, const void* dataptr ) = 0;//�޸�
	private:
		void _FlushIndiesBuffer( uint offset, uint indiesCount, uint typesize, const void* buffer );//��ԭ����bufferoffset��ʼ�����indiesCount�����ݣ�bufferΪ��Ҫ��������
	public:
		virtual void ReleaseResource();//�ͷ���Դ
	public:
		//---------------------------------------------------------------------------------------------------------
		template<typename T>
		void FlushIndiesBuffer( uint indiesCount, const T* buffer )
		{
			_FlushIndiesBuffer( 0, indiesCount, sizeof(T), buffer );
		}
		//---------------------------------------------------------------------------------------------------------
		template<typename T>
		void FlushIndiesBuffer( uint offset, uint indiesCount, const T* buffer)
		{
			_FlushIndiesBuffer( offset, indiesCount, sizeof(T), buffer );
		}
	public:
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetDrawCount(uint count)//���ÿɻ��Ƶĸ��������ͻ��ƿ���
		{
			m_DrawCount = count;
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE uint DrawCount() const
		{
			return m_DrawCount;//��������û���������Ʋ�һ��
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void BindBuffer()
		{
			if ( s_CurrentHandle != m_BufferHandle )
			{
				s_CurrentHandle = m_BufferHandle;
				_DoBindBuffer( m_BufferHandle );
			}
		}
		//---------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetMemoryUseage( BufferObjectStatus::MemoryUseage mu )
		{
			m_eMemoryUseage = mu;
		}
	};
}