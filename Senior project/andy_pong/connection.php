<?php
	
	$connection = mysqli_connect("localhost","andy","password","SmallTalk");

	
	if(!$connection)
	{

		echo "Connection Error ".mysqli_connect_error();
	
	}


	else
	{
	
		$email = urldecode($_POST['Var1']); //Student' email

		$password = urldecode($_POST['Var2']); // Student's password

		//Search for their credentials in the DB.
		$result = $connection ->query("SELECT email from Students where email='$email' and PW='$password'");

	
	if( mysqli_num_rows($result) == 1){

		echo "success";
	}

	else
	{
		echo "Incorrect email or password";
	
	}

	
}


?>
	
