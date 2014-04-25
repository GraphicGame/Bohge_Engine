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



//////////////////////////////////////////
//		������shader��ܺͻ���	   //
//////////////////////////////////////////
#pragma once
#include "Predefine.h"
#include "VariableType.h"
#include <vector>
#include <string>
#include <list>

namespace BohgeEngine
{
	class Attributes
	{
	public:
		enum ShaderAttributes//shader�ڲ���ֵ��Ԥ���ַ
		{
			ATTRIBUTE_POSITION = 0,     // 0
			ATTRIBUTE_BONEINDICES,      // 1
			ATTRIBUTE_BONEWEIGHTS,      // 2
			ATTRIBUTE_NORMAL,           // 3
			ATTRIBUTE_TANGENT,          // 4
			ATTRIBUTE_BINORMAL,         // 5
			ATTRIBUTE_TEXCOORD0,        // 6
			ATTRIBUTE_TEXCOORD1,        // 7
			ATTRIBUTE_COLOR0,           // 8
			ATTRIBUTE_COLOR1,           // 9

			ATTRIBUTE_USER_0,
			ATTRIBUTE_USER_1,
			ATTRIBUTE_USER_2,
			ATTRIBUTE_USER_3,
			ATTRIBUTE_USER_4,
			ATTRIBUTE_USER_5,
			ATTRIBUTE_USER_6,
			ATTRIBUTE_USER_7,

			ATTRIBUTE_COUNT,
		};
	private:
		struct StrShaderAttributes	//��ʼ��shader�ڲ�attribuyes�ṹ��
		{
		public:
			std::string			strName;
			int					nLocation;//ʵ�ʵ����Ե�ַ
			ShaderAttributes	AttributeName;//��������
			StrShaderAttributes( const std::string& name, uint location, ShaderAttributes attr )
				: strName(name),
				nLocation(location),
				AttributeName(attr)
			{
			}
			StrShaderAttributes( const StrShaderAttributes& input )
				: strName(input.strName),
				nLocation(input.nLocation),
				AttributeName(input.AttributeName)
			{
			}
		};
		typedef std::vector<StrShaderAttributes> AttributeList;
	private:
		int					m_Location;
		AttributeList		m_AttributesList;
	public:
		Attributes()
			:m_Location(0)
		{
			m_AttributesList.reserve( ATTRIBUTE_COUNT );
			for ( int i = 0 ; i < ATTRIBUTE_COUNT ; i ++ )
			{
				m_AttributesList.push_back( StrShaderAttributes( "", -1, static_cast<ShaderAttributes>(i) ) );
			}
		}
		BOHGE_FORCEINLINE void PushAttribute( const std::string& name, ShaderAttributes attr )
		{
			m_AttributesList[static_cast<int>(attr)] = StrShaderAttributes( name, m_Location, attr);
			m_Location ++;
		}
		BOHGE_FORCEINLINE uint GetLocation(ShaderAttributes attr)
		{
			return m_AttributesList[static_cast<int>(attr)].nLocation;
		}
		BOHGE_FORCEINLINE uint GetLocation( int index )
		{
			return m_AttributesList[index].nLocation;
		}
		BOHGE_FORCEINLINE std::string& StringAttribute( int index )
		{
			return m_AttributesList[index].strName;
		}
		BOHGE_FORCEINLINE uint Size() const
		{
			return m_AttributesList.size();
		}
	};




	//---------------------------------------------------------------------
	class Shader
	{
	protected:
		handle					m_hShaderProgram;
		Attributes				m_Attributes;
	private:
		Shader( const Shader& s ){}//���ܿ���
		Shader& operator = ( Shader& s ){}
	protected:
		/*ʹ�����shader����shader����Ҫ��ȫ�����ԺͲ���*/
		virtual void _SetParameters() = 0;
	public:
		Shader();
		virtual ~Shader();
		/*��ʼ�����shader*/
		virtual bool Initialization() = 0;
		void UsingShader();
		Attributes& GetAttributesMap()
		{
			return m_Attributes;
		}
	};




	//---------------------------------------------------------------------
	class IRenderNode;
	class Material;
	class MaterialShader : public Shader //һ��Ϊmaterial׼�����Զ���д������shader
	{
	protected:
		const Material*		m_pMaterial;
	public:
		MaterialShader():m_pMaterial(NULL){}
		virtual ~MaterialShader(){};
		BOHGE_FORCEINLINE void SetParamNode(const Material* material)
		{
			m_pMaterial = material;
		}
	};
}