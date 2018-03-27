<?php

	$connection = mysqli_connect("localhost","andy","password","SmallTalk");
	
	if(!$connection)
	{
		echo "Connection Error ".mysqli_connect_error();
	}
	//The name of the new table.
	$Smalltalk = $_POST['presentation'];
	
	//Creates a table with the submitted name
	$sql = "CREATE TABLE $Smalltalk(First VARCHAR(255) NOT NULL,Last VARCHAR(255) NOT NULL,email VARCHAR(255) NOT NULL)";

	//Outputs the appropriate message 
	if ($connection->query($sql) === TRUE)
	{
    		echo "Table $Smalltalk created successfully";
	}
	else
	{
    		echo "Error creating table: " . $connection->error;
	}
 ?>
