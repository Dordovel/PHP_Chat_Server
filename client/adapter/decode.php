<?php
	require_once("../../classes/SSL.php");

	$ssl = new SSL();

	if(($index = array_search("--decode", $argv)) !== false)
	{
		echo $argv[$index + 1];
	}
	elseif(($index = array_search("--encode", $argv)) !== false)
	{
		echo $argv[$index + 1];
		echo $argv[$index + 2];
	}
	elseif(($index = array_search("--generate", $argv)) !== false)
	{
		if(strpos("private", $argv[$index + 1]) !== false)
		{
			echo $ssl->generate_private_rsa_key();
		}
		elseif(strpos("public", $argv[$index + 1]) !== false)
		{
			echo $ssl->generate_public_rsa_key();
		}
	}
?>
