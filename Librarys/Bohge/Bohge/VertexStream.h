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
#include "BufferObjectStatus.h"
#include "VertexStreamLayout.h"
#include "VertexCompress.h"

#include <vector>

namespace BohgeEngine
{
	class VertexStream
	{
	private:
		struct VertexPusher//����ѹե�ṹ��
		{
			VertexCompress::VertexCompressFunction		CompressPtr;//ѹ������ָ��
			byte										PointerStride;//��ַƫ�Ƶ�byteλ
			byte										Count;
			VertexPusher():CompressPtr(NULL),PointerStride(0),Count(0){}
		};
	private:
		typedef std::vector<VertexPusher> VertexPusherVector;
		typedef std::vector<byte> Buffer;
	private:
		VertexStreamLayout		m_Layout;//���ݹ���
		VertexPusherVector		m_VertexPusherVector;
		uint					m_nVertexCount;//�ߴ�
		byte					m_nChunkStride;//ÿһ�����ݵĴ�С
		Buffer					m_Buffer;
	public:
		VertexStream(void);
		~VertexStream(void);
	private:
		void _PushVertexType( Attributes::ShaderAttributes sa, BufferObjectStatus::DataType dt, int count );//���buffer������ count��ÿһ������float������
	public:
		void CreateBuffer( uint size );//����buffer
	public:
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		void PushVertexType( Attributes::ShaderAttributes sa, BufferObjectStatus::DataType dt )//��������ѹ���������������͵�����
		{
			_PushVertexType( sa, dt, T::Size() );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		void SetVertexData( Attributes::ShaderAttributes sa, int index, const T& data )
		{
			VertexPusher& pusher = m_VertexPusherVector[sa];
			ASSERT( T::Size() == pusher.Count );
			pusher.CompressPtr( data.GetVectorArray(), &m_Buffer[index*m_nChunkStride+pusher.PointerStride], pusher.Count );
		}
	public:
		BOHGE_FORCEINLINE const void* GetBufferData() const
		{
			return &m_Buffer[0];
		}
		BOHGE_FORCEINLINE uint GetVertexStride() const
		{
			return m_nChunkStride;
		}
		BOHGE_FORCEINLINE uint GetVertexCount() const
		{
			return m_nVertexCount;
		}
		BOHGE_FORCEINLINE const VertexStreamLayout& GetLayout() const
		{
			return m_Layout;
		}
	};
}
