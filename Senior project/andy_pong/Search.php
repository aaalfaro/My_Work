<?php


	$connection = mysqli_connect("localhost","andy","password","SmallTalk");

	
	if(!$connection)
	{
		echo "Connection Error ".mysqli_connect_error();
	}

	
	$students = $_POST['email'];
	
	$array = explode(',',$students);//Seperates the strings in the textbox are now an array of strings
	
	for($i=0; $i < count($array);$i++) //Iterate through the array of strings
	{
		//Look up the records of each student in the array
		$result = $connection ->query("SELECT Firstname, Lastname, PD, Talks from Students where email='$array[$i]'");

		if( mysqli_num_rows($result) == 1)
		{
			while($row = $result->fetch_assoc()) //Displays their first name, last, number of points, and lectures attended
			{
				echo $row["Firstname"]." ".$row["Lastname"]." ".$row["PD"]." Talks: ".$row["Talks"];
			}

			echo "<br> <br>";
		}
		else
		{
			echo "Student not found"; //If student is not found in the DB.
		}
	}

?>
