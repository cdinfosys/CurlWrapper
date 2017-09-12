<?php
    print("Start\n");
    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $inputVal = $_POST["UploadValue"];
        if (is_null($inputVal))
        {
            $storeData = -1;
            $storeError = "'UploadValue' key missing in POST values";
        }
        else
        {
            $jsonData = json_decode($inputVal);
            $uploadedValue = $jsonData-> { "UploadValue" };
            if (is_null($uploadedValue))
            {
                $storeData = -1;
                $storeError = "[UploadValue] is missing from JSON: ".$inputVal;
            }
            else if (is_numeric($uploadedValue))
            {
                $intVal = intval($uploadedValue);
                if (($intVal < 0) || ($intVal > 9999))
                {
                    $storeData = -1;
                    $storeError = "The number must be in the range [0..9999]";
                }
                else
                {
                    $storeData = $intVal;
                    $storeError = null;
                }
            }
            else
            {
                $storeData = -1;
                $storeError = "Input value must be numeric: ".$uploadedValue;
            }
        }
    }
    else
    {
        $storeData = -1;
        $storeError = "Use POST to send values";
    }

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

    $sql = "
        INSERT INTO puxuluj_DemoDB.CurlWrapperTesting(CurlWrapperTestingID, DataValue, ErrorMessage) VALUES(1, ?, ?)
        ON DUPLICATE KEY UPDATE DataValue = ?, ErrorMessage = ?
    ";
    $statement = $conn->prepare($sql);
    $statement->bind_param("isis", $storeData, $storeError, $storeData, $storeError);
    $statement->execute();
    $statement->close();
    $conn->close();

    if (is_null($storeError))
    {
        print("OK");
    }
    else
    {
        print($storeError);
    }
?>
