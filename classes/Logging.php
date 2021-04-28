<?php

    class LoggingType
    {
        const CONSOLE = 1;
        const FILE = 2;
    }

    class Logging
    {
        public $_type;

        public function __construct($type)
        {
            $this->_type = $type;
        }

        public function __call($name, $value)
        {
            if($this->_type == LoggingType::FILE && $name == "set_filename")
                $this->_filename = current($value);
        }

        private function get_log($message)
        {
            return date("d:m:Y G:i:s"). " ->  ". $message. "\n";
        }

        public function log($message)
        {
            switch($this->_type)
            {
                case LoggingType::CONSOLE:
                    echo $this->get_log($message);
                break;

                case LoggingType::FILE:
                {
                    if(isset($this->_filename))
                        file_put_contents($this->_filename, $this->get_log($message),
                                            FILE_APPEND);
                }

                default:break;
            }
        }
    }
?>