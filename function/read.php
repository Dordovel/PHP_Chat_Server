<?php
    function read_from_socket($socket)
    {
        $message = socket_read($socket, 1024,PHP_BINARY_READ);

        socket_set_nonblock($socket);

        while($buffer = socket_read($socket, 2048,PHP_BINARY_READ))
            $message .= $buffer;

        return $message;
    }
?>
