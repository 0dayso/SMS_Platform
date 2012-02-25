<?php
//Functions
	/*
	说明：
	^表示这符开始(元字符)
	$表示字符结尾(元字符)
	13为普通字符
	[0-9]表示一个0-9的数字
	{9} 表示匹配9次
	后面的号码，如果是13开头的，中间有9个数字，并结尾，则符合条件
	*/
function PhoneNumberDetect( $number ) {
	$ResDetect = false;
	
	if ( !( ''==$number || 0 == count($number) ) )
		$ResDetect = preg_match("/^13[0-9]{1}[0-9]{8}$|15[0189]{1}[0-9]{8}$|189[0-9]{8}$/",$number);
	 
	return $ResDetect;
}
?>
