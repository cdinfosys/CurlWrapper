<?php
    $serverName = "localhost";
    $userName = "puxuluj_DemoUser";
    $password = "iPRDQWU8N1b0UmEsDZs5";
    $dbname = "puxuluj_DemoDB";

    // Create connection
    $conn = new mysqli($serverName, $userName, $password, $dbname);

    // Check connection
    if ($conn->connect_error)
    {
        die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT DataValue, ErrorMessage FROM puxuluj_DemoDB.CurlWrapperTesting WHERE CurlWrapperTestingID = 1";
    $result = $conn->query($sql);

    if ($result->num_rows > 0)
    {
        // output data of each row
        $row = $result->fetch_assoc();
        $resultData = array
        (
            "inputValue" => $row[DataValue],
            "errorMessage" => $row[ErrorMessage]
        );
    }
    else
    {
        $resultData = array
        (
            "inputValue" => -1,
            "errorMessage" => "No test value stored."
        );
    }
    $conn->close();

    header("Content-type: application/json");
    print json_encode($resultData);
?>
