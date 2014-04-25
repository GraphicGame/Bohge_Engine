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
#include "Predefine.h"
#include "BaseShader.h"



#include <vector>
#include <list>

namespace BohgeEngine
{
	//-------------------------------------------------------------------
	class VertexStreamLayout	//����Ĳ��ַ�ʽ��ʹ��Vertex�������ֱ�����
	{
	public:
		#pragma pack(push,1)
		class Layout
		{
		private:
			Attributes::ShaderAttributes	m_eAttributes;//���������
			BufferObjectStatus::DataType	m_eDataType;//�������ݵ�����
			bool							m_isNormalize;//�Ƿ�淶���ģ�һ��Ϊfalse
			byte							m_uSize;//ÿ���������ݵĸ���
			byte							m_uStride;//ÿ�����ݶε�biteƫ����
		public:
			Layout()
				:m_eAttributes((Attributes::ShaderAttributes)-1),
				m_eDataType((BufferObjectStatus::DataType)-1),
				m_isNormalize(false),
				m_uSize(0),
				m_uStride(0)
			{
			}
			Layout( Attributes::ShaderAttributes attr, BufferObjectStatus::DataType type, byte size, bool isnormal )
				:m_eAttributes(attr),
				m_eDataType(type),
				m_uSize(size),
				m_isNormalize(isnormal),
				m_uStride(0)
			{
				ASSERT( 0 != size );
				switch ( type )
				{
				case BufferObjectStatus::DT_FLOAT: m_uStride = size * sizeof(float); break;
				case BufferObjectStatus::DT_HALF_FLOAT: m_uStride = size * sizeof(short); break;
				case BufferObjectStatus::DT_INT_8_8_8_8: m_uStride = sizeof(int); break;
				case BufferObjectStatus::DT_UINT_8_8_8_8: m_uStride = sizeof(int); break;
				case BufferObjectStatus::DT_INT_10_10_10_2: m_uStride = sizeof(int); break;
				case BufferObjectStatus::DT_UINT_10_10_10_2: m_uStride = sizeof(int); break;
				default: ASSERT(false && "Unkown vertex data type !\n");
				}
			}
		public:
			BOHGE_FORCEINLINE bool isActive()
			{
				return 0 != m_uSize;
			}
			BOHGE_FORCEINLINE Attributes::ShaderAttributes GetAttributes() const
			{
				return m_eAttributes;
			}
			BOHGE_FORCEINLINE byte Size() const
			{
				return m_uSize;
			}
			BOHGE_FORCEINLINE BufferObjectStatus::DataType DataType() const
			{
				return m_eDataType;
			}
			BOHGE_FORCEINLINE bool isNormalize() const
			{
				return m_isNormalize;
			}
			BOHGE_FORCEINLINE byte Stride() const
			{
				return m_uStride;
			}
		};
		#pragma pack(pop)
	private:
		typedef std::vector<Layout>	LayoutVector;
		typedef std::list<Layout> LayoutList;//Ϊ�˿��ٵ�����Ⱦ�����ﻹ����list����
	private:
		LayoutVector		m_LayoutArray;
		LayoutList			m_LayoutList;
	public:
		VertexStreamLayout()
		{
			m_LayoutArray.resize( Attributes::ATTRIBUTE_COUNT );
		}
	public:
		//����һ������
		BOHGE_FORCEINLINE void SetVertexLayout( Attributes::ShaderAttributes attr, BufferObjectStatus::DataType type, byte size, bool isnormal )
		{
			m_LayoutArray[attr] = Layout(attr,type,size,isnormal);
			m_LayoutList.push_back( m_LayoutArray[attr] );
		}
		BOHGE_FORCEINLINE const Layout& GetVertexLayout(  Attributes::ShaderAttributes attr )
		{
			return m_LayoutArray[attr];
		}
		BOHGE_FORCEINLINE const std::list<Layout>* GetActivedLayoutList()
		{
			return &m_LayoutList;
		}
	};
	typedef std::list<VertexStreamLayout::Layout> LayoutList;
}