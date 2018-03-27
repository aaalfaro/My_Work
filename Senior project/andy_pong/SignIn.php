<?php

	$connection = mysqli_connect("localhost","andy","password","SmallTalk");
	if(!$connection)
	{
		echo "Connection Error ".mysqli_connect_error();
	}
	else
	{
		$email = urldecode($_POST['Var2']);
		$smalltalk = urldecode($_POST['Var1']); //The value in the QR code
		//Search for the student in the lecture table to see if he has already signed in.
		$result = $connection ->query("SELECT email from $smalltalk where email ='$email'"); 
		if( mysqli_num_rows($result) == 0) //Signs the student in if he wasn't found in the table
		{
			$Signin = $connection ->query("INSERT INTO $smalltalk(First,Last,email) SELECT Firstname,Lastname,email from Students WHERE email ='$email'");
			if(!$Signin)
			{
				echo "This Smalltalk does not exist";
			}
			else if(strpos($smalltalk, '2PD')!== false) //Some lectures are worth 2 points. gives students the appropriate amount of points.
			{
				$PD = $connection ->query("UPDATE Students SET PD = IF(PD is null, 1, PD + 2) WHERE email ='$email'"); //Increment points by 2
				$Talk = $connection ->query("Update Students SET Talks= IF(Talks is null, '$smalltalk',concat(Talks,', $smalltalk')) where email = '$email'");
				echo "You are signed in";
			}

			else
			{
				$PD = $connection ->query("UPDATE Students SET PD = IF(PD is null, 1, PD + 1) WHERE email ='$email'"); //Increment points by 1
				$Talk = $connection ->query("Update Students SET Talks= IF(Talks is null, '$smalltalk',concat(Talks,', $smalltalk')) where email = '$email'");
				echo "You are signed in";
			}
		}
	else
		{
			echo "You are already signed in";
		}
	}
?>
