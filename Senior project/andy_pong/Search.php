<?php


	$connection = mysqli_connect("localhost","andy","password","SmallTalk");

	
if(!$connection)

	{
		
		echo "Connection Error ".mysqli_connect_error();

	}

	
$students = $_POST['email'];
	
$array = explode(',',$students);
	
for($i=0; $i < count($array);$i++)

	{

		$result = $connection ->query("SELECT Firstname, Lastname, PD, Talks from Students where email='$array[$i]'");

		if( mysqli_num_rows($result) == 1)

		{

			while($row = $result->fetch_assoc())

			{

				echo $row["Firstname"]." ".$row["Lastname"]." ".$row["PD"]." Talks: ".$row["Talks"];
 
			}

			echo "<br> <br>";

		}

		else

		{

			echo "Student not found";

		}
	
	}

?>