<?php 
	session_start();
?>
<?php
if($_SESSION["login"] != "YES")
{
	echo "<script languge=javascript> alert('您没有登录，无权访问，请先登录！');
			location.href='index.php'</script>";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"><!-- InstanceBegin template="/Templates/tmplet.dwt.php" codeOutsideHTMLIsLocked="true" -->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>短信群发管理系统</title>
<link type="text/css" rel="stylesheet" href="style/style.css" />

<!-- InstanceBeginEditable name="EditRegion2" -->
<link type="text/css" rel="stylesheet" href="style/common.css" />
<script language="javascript" src="lib/jquery.js"></script>
<script language="JavaScript" type="text/javascript">
$(document).ready(function() {
	setInterval(CheckNew, 3000);
});

function CheckNew() {
	$('#button2check').trigger('click'); 
};


function CheckNewMsg(time) {
	$.post(
	  'check_NewMsg.php',
	  { time:time },
	  function (data) //回传函数
	  {
		var myjson='';
		eval('myjson=' + data + ';');
		//alert("myjson.flag is :("+myjson.flag+")");
		//alert("myjson.time is :("+myjson.time+")");
		//alert("myjson.check_new is :("+myjson.check_new+")");
		if( myjson.flag!=0 ) {
			if( myjson.check_new > 0 ) {
				//新短信
				$('#DisplayNewMsgs').html('<a href="recv_box.php">收到新短信，点击查看。</a>');
				//alert('There are new Msgs!');
			} else {
				$('#DisplayNewMsgs').html('');
			}
		} else {
			alert("对不起，查询新短信失败！");
		}
	  }
	);
};


function Have_Read(id) {
	$('#flag_receive'+id).html("waiting..."); 
	$.post(
	  'have_read.php',
	  { MsgId:id },
	  function (data) //回传函数
	  {
		var myjson='';
		eval('myjson=' + data + ';');
		//alert("myjson.flag is :("+myjson.flag+")");
		//alert("myjson.id is :("+myjson.id+")");
		if( myjson.flag!=0 ) {
			$('#flag_receive'+id).remove();
			//$('#DisplayNewMsgs').html('');
		} else {
			alert("对不起，操作不成功！");
		}
	  }
	);
};

function AjaxDelete( table, id )
{
	$('#tr_tag'+id).html("waiting..."); 
	$.post(
	  'ajax_delete.php',
	  { table:table, MsgId:id },
	  function (data) //回传函数
	  {
		var myjson='';
		eval('myjson=' + data + ';');
		//alert("myjson.flag is :("+myjson.flag+")");
		//alert("myjson.id is :("+myjson.id+")");
		if( myjson.flag!=0 && myjson.flag!=0 ) {
			$('#tr_tag'+id).remove()  ;
		} else {
			alert("对不起，操作不成功！");
		}
	  }
	);
};




</script>
<!-- InstanceEndEditable -->
</head>
<body>
<div id="container">
  <div id="head"> </div>
  <div id="menu">
    <ul>
      <li id="first_li"><a href="group.php">发送短信</a></li>
      <li><a href="send_box.php">发件箱</a></li>
      <li><a href="recv_box.php">收件箱</a></li>
      <li><a href="mynumber.php">管理电话薄</a></li>
      <li><a href="autoreply.php">添加自动回复</a></li>
      <li><a href="autoreply_ctl.php">管理自动回复</a></li>
      <li><a href="toadmin.php">联系管理员</a></li>
      <li><a href="logout.php">注销</a></li>
    </ul>
  </div>
  <div id="crumbs">
	短信管理 &gt;&gt;&gt; 
	<!-- InstanceBeginEditable name="EditRegion3" --> 
	  收件箱	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" --> 
<?php 
	require_once('connecting.php');
	$SqlTable = "receivemessage";
	
	$current_page = ( isset($_GET['page']) ) ? $_GET['page'] : 1;
	
	$start_limit = ( $current_page - 1 ) * $page_items;
	
	$query = "select * from `$SqlTable`";
	$result = mysql_query( $query );
	$sum_rows_all = mysql_num_rows($result);
	
	$tatal_pages = ceil( $sum_rows_all/$page_items );
	
	$cur_page_name = $_SERVER["PHP_SELF"];
?>


<?php
	//清空表
	$table = trim($_GET["table"]);
	if($table != "") {
		mysql_query( "TRUNCATE TABLE  `$table`;" ) or die(mysql_error());
		echo "<script languge=javascript> alert('执行成功！'); 
					location.href='$cur_page_name'</script>";
	} 
?>

<?php
	//删除
	$del_id = trim($_GET["del_id"]);
	if( $del_id != "" ) {
		$sql = "delete from `".$SqlTable."` where `idreceive` ='$del_id'";
		mysql_query( $sql ) or die(mysql_error());
		echo "<script languge=javascript> alert('删除成功！'); 
					location.href='$cur_page_name'</script>";
	} 
?>
  <table class=i_table cellSpacing=1 cellPadding=4 width="100%" align=center>
              <TR>
                <TD class=head colSpan=4>收件箱   
					<span id=button2check 
					onclick="CheckNewMsg('<?=time()?>')" style="display:none;">
					查询</span>
					<span id=DisplayNewMsgs style="cursor:text;" class=a_button style="margin-left:20em;"></span>				</TD>
              </TR>
            <TBODY id=cate_a0>
              <TR class=c>
                <td align="center" width="10%">姓名/号码</td>
                <td align="center" width="12%">接收时间</td>
                <td align="center" width="63%">短信内容</td>
                <td align="center" width="15%">操作</td>
              </tr>

<?php
	$query = "select * from `$SqlTable` 
				ORDER BY `flagreceive` DESC, `timereceive` DESC  
				LIMIT $start_limit , $page_items ";
	$result = mysql_query( $query );
	$sum_rows = mysql_num_rows($result);
	if($sum_rows < 1) {
		echo "<tr class='b'><td align='center' colspan='4'>还没有任何短信!</td></tr>";
	} else {
		while( $info = mysql_fetch_array( $result ) )
		{
			if($info != "") {
				$name = Number2Name( $info['numreceive'] );
				$ContractNo = PhoneNumberDetect($info['numreceive']);
				
				$FlagReceive = "";
				if( $info['flagreceive'] != 0 ) {
					$FlagReceive = "<span title='标记为已读' class=flag_receive id=flag_receive".
									$info['idreceive'].
									" onclick='Have_Read(\"".$info['idreceive']."\")'>未阅</span>";
				}
?>
			   
			   <TR class=b onMouseOver="this.className='right_over';" 
  onmouseout="this.className='b';" id="tr_tag<?=$info['idreceive']?>"><!--<span class='flag_receive'>-->
                  <td align="center"><strong><?=$name?></strong><br>
				  <?php 
				  if($name != $info['numreceive'])  {
				  	echo "<small>".$ContractNo."</small>";
				  } else {
				  	if($ContractNo) {
						echo "<a id='a_in_tab' href='modify.php?content=".
									$ContractNo."'>加入电话薄</a>";
					}
				  }
				  ?>				  </td>
                  <td align="center"><?=$info['timereceive']?></td>
                  <td style="padding-left:1em; padding-right:1em;">
				  	<?=$FlagReceive?>
				  <?=$info['contentreceive']?>				  </td>
                  <td align="center">
					<a href="send.php?number=<?=$info['numreceive']?>&recv_id=<?=$info['idreceive']?>">
						<span class=a_button title='回复此短信'>回复</span>					</a>&nbsp;
						<span class=a_button title='删除此短信' 
						onclick="AjaxDelete('<?=$SqlTable?>', '<?=$info['idreceive']?>')">
						删除</span>				  </td>
                </tr>
<?php 
			}
		}
	}
?>
				<TR class=c >
                <td align="center" colspan="4">
				  <table border="0" cellSpacing=0 cellPadding=0 width="100%" align=center>
					  <tr class=c>
						<td width="15%"> 总共&nbsp;<font color="black"><b>
						  <?=$sum_rows_all?>
						  </b></font>&nbsp;条记录 </td>
						<td width="20%" align="left">
							<a href="<?=$cur_page_name?>?table=<?=$SqlTable?>">
							<span class=a_button>清空</span>							</a>						</td>
						<td width="65%" align="right" style="padding-right:2em;">
						<a href="<?=$cur_page_name?>"> 首页</a>|
						  <?php if ($current_page > 1) {  ?>
						  <a href="?page=<?=($current_page - 1)?>">
						  <?php } ?>
						  上一页</a>|
						  <?php if ($current_page < $tatal_pages) { ?>
						  <a href="?page=<?=($current_page + 1)?>">
						  <?php }  ?>
						  下一页</a>|
						  <?php if ($current_page < $tatal_pages) {  ?>
						  <a href="?page=<?=$tatal_pages?>">
						  <?php }  ?>
						  尾页</a><!--| <a  onclick="javascript:return p_del()" 
								href="?delete=all">删除本页</a> --></td>
					  </tr>
					</table>					</td>
              </TR>
            </TBODY>
          </table>
<?php
	mysql_close($member);
?>		
</table>
  <!-- InstanceEndEditable --> </div>
  <div id="footer"><br />
    &copy;2010 　Designed By <a href="http://hi.baidu.com/rjz78"><cite>Don Ren</cite><img src="images/stone.gif" alt="石三" style="margin-left:15px; border:none;" /></a><br />
    <br />
  </div>
</div>
</body>
<!-- InstanceEnd --></html>
