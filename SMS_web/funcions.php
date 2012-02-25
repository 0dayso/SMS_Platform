<?php 
/*
 * A platform based on B/S mode with enough features to be as a SMS server.
 * <SMS_Platform> Copyright (C) <2012>  <Jiangzhe Ren>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Email : jiangzhe78@gmail.com
 */
?>
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
