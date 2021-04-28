<?php
	require_once("../classes/SSL.php");

	$ssl = new SSL();

	function read_key()
	{
		$key = "";

		while($line = fgets(STDIN, 2048))
			$key .= $line;

		return $key;
	}

if(($index = array_search("--decode", $argv)) !== false)
	{
		if(($indexPipe = array_search("--pipe", $argv)) !== false)
		{
			$key = read_key();

			$msg = file_get_contents($argv[$indexPipe + 1]);
			file_put_contents($argv[$indexPipe + 1], $ssl->decrypt_rsa($key, $msg));
		}
	}
	elseif(($index = array_search("--encode", $argv)) !== false)
	{
		if(($indexPipe = array_search("--pipe", $argv)) !== false)
		{
		    $key = read_key();

			$msg = file_get_contents($argv[$indexPipe + 1]);
			file_put_contents($argv[$indexPipe + 1], $ssl->encrypt_rsa($key, $msg));
		}
	}
	elseif(($index = array_search("--generate", $argv)) !== false)
	{
		echo $ssl->generate_private_rsa_key();
		echo $ssl->generate_public_rsa_key();
	}
?>
