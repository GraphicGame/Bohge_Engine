
--��͸߶�
MinHeight = 0.0

--��ʼ��Lua�����ó�ʼ��������������Ҫ�Ŀ����
function RegisterScript()
	Script:LoadClasses( CLASS_VECTOR3B )
	return true
end


function Initialization()
	--�������ͣ����þ�γ���������ģ��ԭ���λ�ã�����������ȷ��һ��ƽ�棬��λģ��
	--this:SetContorlPoint( Vector3b( 40.0711251607, 116.2739160731, 36.0 ), Vector3b( -102.345, 0, -76.022) )--z3
	--this:SetContorlPoint( Vector3b( 40.0713604288, 116.2764820990, 36.0 ), Vector3b( 109.315, 0, -14.713) )--z5
	--this:SetContorlPoint( Vector3b( 40.0696617371, 116.2746951533, 36.0 ), Vector3b( -102.345, 0, 101.1) )--z7
	
	--this:SetContorlPoint( Vector3b( 40.07112260293, 116.27391831935, 45.13 ), Vector3b( -102.52800, 0, -71.487015) )
	--this:SetContorlPoint( Vector3b( 40.06965632422, 116.27469139221, 45.13 ), Vector3b( -101.78201, 0, 101.99501) )
	--this:SetContorlPoint( Vector3b( 40.07183842098, 116.27622461716, 45.13 ), Vector3b( 105.19602, 0, -73.557022) )
	
	this:SetContorlPoint( Vector3b( 40.07112260293, 116.27391831935, 45.13 ), Vector3b( -102.52800, 0, -71.487015) )
	this:SetContorlPoint( Vector3b( 40.06965632422, 116.27469139221, 45.13 ), Vector3b( -102.52800, 0, 101.99501) )
	this:SetContorlPoint( Vector3b( 40.07183842098, 116.27622461716, 45.13 ), Vector3b( 105.19602, 0, -71.487015) )
end


function Clearup()

end


function Update( millisecond )
	return true
end