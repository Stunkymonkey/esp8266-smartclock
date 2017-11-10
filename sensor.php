<?php

$servername = "localhost:3307";
$username = "smonkey";
$password = "au0noh4ohCaicaef";
$dbname = "smonkey_grafana";
$table = "esp8266";
$token = "gPyYUuoUivdni6dzqgGNXfonY";
$notificationUrl = "https://api.telegram.org/bot494953698:AAHws8cU6F9yCfVb32583wZCDrNzh2ceNTY/sendMessage?chat_id=60195390&text=Bitte%20L%C3%BCften!%F0%9F%92%A8";
$maxValue = 70;

//Make sure that it is a POST request.
if(strcasecmp($_SERVER['REQUEST_METHOD'], 'POST') != 0){
    throw new Exception('Request method must be POST!');
}

//Make sure that the content type of the POST request has been set to application/json
$contentType = isset($_SERVER["CONTENT_TYPE"]) ? trim($_SERVER["CONTENT_TYPE"]) : '';
if(strcasecmp($contentType, 'application/json') != 0){
    throw new Exception('Content type must be: application/json');
}

$headers = getallheaders();
$request_token = $headers['Token'];
if (!($token == $request_token)){
    header('HTTP/1.0 403 Forbidden'); 
    die('You are not allowed to access this.'); 
}


//Receive the RAW post data.
$content = trim(file_get_contents("php://input"));

//Attempt to decode the incoming RAW post data from JSON.
$decoded = json_decode($content, true);

//If json_decode failed, the JSON is invalid.
if(!is_array($decoded)){
    throw new Exception('Received content contained invalid JSON!');
}

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
} 

//Process the JSON.
//echo '<pre>'; print_r($decoded); echo '</pre>';

// get columns from request
$columns = array_keys($decoded);

$temp_i = array_search('temp', $columns);
$hum_i = array_search('humidity', $columns);

// get values
$escaped_values = array_map(array($conn, 'real_escape_string'), array_values($decoded));

// insert data
$sql = "INSERT INTO $table(temperature, humidity) VALUES ('$escaped_values[$temp_i]', '$escaped_values[$hum_i]')";

// execute command
if (mysqli_query($conn, $sql)) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}

//close connection
mysqli_close($conn);

if((int)$escaped_values[$hum_i] >= maxValue){
    $contents = file_get_contents($notificationUrl);
}

?>
