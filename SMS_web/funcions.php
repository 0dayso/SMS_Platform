<?php
//Functions
	/*
	˵����
	^��ʾ�����ʼ(Ԫ�ַ�)
	$��ʾ�ַ���β(Ԫ�ַ�)
	13Ϊ��ͨ�ַ�
	[0-9]��ʾһ��0-9������
	{9} ��ʾƥ��9��
	����ĺ��룬�����13��ͷ�ģ��м���9�����֣�����β�����������
	*/
function PhoneNumberDetect( $number ) {
	$ResDetect = false;
	
	if ( !( ''==$number || 0 == count($number) ) )
		$ResDetect = preg_match("/^13[0-9]{1}[0-9]{8}$|15[0189]{1}[0-9]{8}$|189[0-9]{8}$/",$number);
	 
	return $ResDetect;
}
?>
