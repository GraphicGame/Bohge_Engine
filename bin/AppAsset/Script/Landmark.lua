

--��ʼ��Lua�����ó�ʼ��������������Ҫ�Ŀ����

function RegisterScript()
	Script:LoadClasses( LUALIB_TABLE )
	Script:LoadClasses( CLASS_VECTOR3F )
	return true
end


function Initialization()
	this:AddLandmark( "�ռ�Ӧ�ù����뼼������", -116.640029907, 0.0,  -79.0599822998 )--���Ӹ���Ȥ��Ŀ��㣬��һ������ΪĿ�����֣��ڶ���Ϊ����
	this:AddLandmark( "��������", -142.640014648, 0.0, 101.36000824 )
	this:AddLandmark( "����о�Ժ", -61.360004425, 0.0, 53.3599853516 )
	this:AddLandmark( "ң�������ֵ����о���", 61.5199966431, 0.0, 40.239982605 )
	this:AddLandmark( "����ң������", 140.239944458, 0.0, 88.8800048828 )
	this:AddLandmark( "��ѧ�о�����¥", 163.239959717, 0.0, -16.7799930573 )
	this:AddLandmark( "��ѧ�о�����¥", 160.039962769, 0.0, 45.9800109863 )
	this:AddLandmark( "�ۺ�¥", 167.639953613, 0.0, -89.259979248 )
end


function Clearup()

end


function Update( millisecond )
	return true
end