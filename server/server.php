<?php

	require_once "../function/read.php";
	require_once "../classes/SSL.php";
	require_once "../classes/Logging.php";
    require_once "../classes/Type.php";
    require_once "../classes/ResponceStatus.php";
	require_once "handlers/Handler.php";

	class ServerAddress
	{
		const HOST = '127.0.0.1';
		const PORT = '7000';
	}

	class Server
    {
        private $_logger;
        private $_serverSocket;
        private $_clients;
        private $_handler;

        public function __construct(Logging $log)
        {
            $this->_logger = $log;
            $this->_clients = array();
        }

        public function init_web_socket()
        {
            $this->_serverSocket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
            if (!socket_set_option($this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, 1))
            {
                $this->_logger->log("Error socket init param");
                socket_close($this->_serverSocket);
                return false;
            }

            if (!socket_bind($this->_serverSocket, ServerAddress::HOST, ServerAddress::PORT))
            {
                $this->_logger->log("Error socket bind");
                socket_close($this->_serverSocket);
                return false;
            }

            if (!socket_listen($this->_serverSocket))
            {
                $this->_logger->log("Error socket listen");
                socket_close($this->_serverSocket);
                return false;
            }

            return true;
        }

        private function get_request($message)
        {
            return Data::decode($message);
        }

        private function close_socket($socket)
        {
            socket_close($socket);
            if($find = array_search($socket, $this->_clients))
                unset($this->_clients[$socket]);
        }

        /**
         * @param $socket
         * @param $request
         * @return Data
         */
        public function call_handler($socket, $request)
        {
            $client = &$this->_clients[$socket];
			return $this->_handler->execute($client, $request);
        }

        public function set_handler(RequestHandler $handler)
        {
            $this->_handler = $handler;
        }

        public function run()
        {
            $clientsSocket[] = $this->_serverSocket;

            $SERVER_IS_RUN = true;

            while ($SERVER_IS_RUN)
            {
                $changedClientSockets = $clientsSocket;

                $selectSocketsExceptionArray = array();
                $selectSocketsWriteEmptyParams = null;
                $readySocketCount = socket_select($changedClientSockets,
                                            $selectSocketsWriteEmptyParams,
                                            $selectSocketsExceptionArray, 10);

                if (empty($readySocketCount)) continue;

                foreach ($changedClientSockets as $changedSocket)
                {
                    if(in_array($this->_serverSocket, $changedClientSockets))
                    {
                        $lastConnectClientSocket = socket_accept($this->_serverSocket);
                        $clientsSocket[] = $lastConnectClientSocket;

                        $index = array_search($this->_serverSocket, $changedClientSockets);
                        unset($changedClientSockets[$index]);

                        continue;
                    }

                    $data = read_from_socket($changedSocket);

                    if(empty($data))
                    {
                        $this->close_socket($changedSocket);

                        if($find = array_search($changedSocket, $clientsSocket))
                            unset($clientsSocket[$find]);

                        continue;
                    }

                    $request = $this->get_request($data);

                    $responce = $this->call_handler($changedSocket, $request);
					socket_write($changedSocket, $responce, strlen($responce));
                }
            }
        }
    }
?>
