<?php

    class Data
    {
        static public function encode($data)
        {
            return json_encode($data);
        }

        static public function decode($data)
        {
            return json_decode($data);
        }
    }
?>