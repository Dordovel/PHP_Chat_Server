<?php

    //ini_set('display_errors', 1);
    //ini_set('display_startup_errors', 1);
    //error_reporting(E_ALL);

    require_once "Connection.php";
    require_once "server.php";
    require_once "../classes/Logging.php";
    require_once "handlers/Handshake.php";
    require_once "handlers/Autorization.php";
    require_once "handlers/GroupList.php";
    require_once "handlers/GroupMessage.php";

    $log = new Logging(LoggingType::FILE);
    $log->set_filename("./log.txt");

    $connection = new Connection("sqlite:serverBD.sqlite", $log);

    $ssl = new SSL();

    $handler = new Handshake($ssl);
    $handler->next(new Authorization($connection, $ssl))
            ->next(new GroupList($connection))
            ->next(new GroupMessage($connection, $ssl));

    $server = new Server($log);
    $server->set_handler($handler);

    if($server->init_web_socket())
        $server->run();
?>
