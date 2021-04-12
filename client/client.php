<?php

	require_once "../function/read.php";
	require_once "../classes/SSL.php";
	require_once "../classes/Type.php";
	require_once "../classes/ResponceStatus.php";
	require_once "../classes/Data.php";

	class ServerAddress
	{
		const HOST = '127.0.0.1';
		const PORT = '7000';
	}

	$ssl = new SSL();

	$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

	$publicRSAKey = $ssl->generate_public_rsa_key();
	$privateRSAKey = $ssl->generate_private_rsa_key();

	socket_connect($socket, ServerAddress::HOST, ServerAddress::PORT);

	$connectionQuery = Data::encode(array("Type"=>Type::HANDSHAKE,
										"Key" => $publicRSAKey));

	socket_write($socket, $connectionQuery, strlen($connectionQuery));

	$data = read_from_socket($socket);

	$response = Data::decode($data);

	$pass = $ssl->encrypt_rsa($response->Key, "admin");
	$log = $ssl->encrypt_rsa($response->Key, "admin");

	$authQuery = Data::encode(array("Type"=>Type::AUTORIZATION,
									"Login" => $log,
									"Password" => $pass));

	socket_write($socket, $authQuery, strlen($authQuery));

	sleep(1);

	$data = read_from_socket($socket);

	if(!$data) exit;

	$response = Data::decode($data);

	if($response->Type == Type::AUTORIZATION
		&& $response->Status == ResponceStatus::OK)
	{
		$groupQuery = Data::encode(array("Type" => Type::GROUP_LIST));

		socket_write($socket, $groupQuery, strlen($groupQuery));

		sleep(1);

		$data = read_from_socket($socket);

		$response = Data::decode($data);

		if ($response->Type == Type::GROUP_LIST)
		{
			foreach ($response->Groups as $index => $group)
			{
				echo ($index + 1), ": Name: ", $group->name,
						" Info: ", $group->info,
						" Creater: ", $group->creator, "\n";
			}

			$input = readline("Select group->");
			$group = $response->Groups[$input - 1]->name;
			echo date('Y-m-d'), "  Select: ", $group, "\n";
			$messageQuery = Data::encode(array('Type' => Type::GROUP_MESSAGES,
							"Group" => $group));

			socket_write($socket, $messageQuery, strlen($messageQuery));

			sleep(1);

			$data = read_from_socket($socket);
			$response = Data::decode($data);

			var_dump($response);
		}
	}
?>
