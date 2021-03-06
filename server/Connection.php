<?php

    class Connection
    {
        /**
         * @var PDO $_pdo
         */
        private $_pdo;
        private $_logger;

        public function __construct($path, Logging $log)
        {
            $this->_logger = $log;
            try
            {
                $this->_pdo = new PDO($path);
                $this->_pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
                $this->_pdo->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
            }
            catch (PDOException $ex)
            {
                $this->_logger->log($ex->getMessage());
            }
        }

        /**
         * @return PDO
         */
        public function connection()
        {
            return $this->_pdo;
        }
    };

?>