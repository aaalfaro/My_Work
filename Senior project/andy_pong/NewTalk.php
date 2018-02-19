<?php


	$connection = mysqli_connect("localhost","andy","password","SmallTalk");

	
if(!$connection)
	
{

		echo "Connection Error ".mysqli_connect_error();
	}


	$Smalltalk = $_POST['presentation'];


	$sql = "CREATE TABLE $Smalltalk(First VARCHAR(255) NOT NULL,Last VARCHAR(255) NOT NULL,email VARCHAR(255) NOT NULL)";


	if ($connection->query($sql) === TRUE)
 
	{

    		echo "Table $Smalltalk created successfully";

	}
 
	else
	{

    		echo "Error creating table: " . $connection->error;

	}

 ?>