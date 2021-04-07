<?php

    class GroupMessage extends RequestHandler
    {
        private $_db;
        private $_ssl;

        const Message = 'Msg';
        const Sender = 'Sender';
        const SendDate = 'Date';
        const Messages = 'Messages';

        public function __construct(Connection $db, SSL $ssl)
        {
            $this->_db = $db;
            $this->_ssl = $ssl;
        }

        private function check_user_subscribe($login, $group)
        {
            $q = "SELECT
                        CASE
                            WHEN count(user_group.id) <= 0
                                THEN 0
                            ELSE 1
                        END isset
                    FROM user_group
                    INNER JOIN groups ON groups.name = '$group'
                    INNER JOIN users ON users.login = '$login'
                    WHERE user_group.group_id = groups.id
                    AND user_group.user_id = users.id";

            return $this->_db->query($q)->fetch(PDO::FETCH_ASSOC)['isset'];
        }

        private function get_group_messages($group, $date)
        {
            $q = " SELECT
                        messages.message,
                        u.login,
                        messages.date
                    FROM messages
                    INNER JOIN groups g on messages.group_id = g.id
                    INNER JOIN users u on messages.user_id = u.id
                    WHERE DATE(messages.date) = DATE('$date')
                    AND g.name = '$group'";

            return $this->_db->query($q)->fetchAll(PDO::FETCH_ASSOC);
        }

        private function convert_messages($pubKey, $messageList)
        {
            $data = array();

            foreach ($messageList as $message)
            {
                $encodeMsg = $this->_ssl->encrypt_rsa($pubKey, $message['message']);

                $data[GroupMessage::Message] = base64_encode($encodeMsg);
                $data[GroupMessage::Sender] = $message['login'];
                $data[GroupMessage::SendDate] = $message['date'];
            }

            return $data;
        }

        private function exec(&$client, $request)
        {
            $responceData = array("Type" => Type::GROUP_MESSAGES,
                                    GroupMessage::Messages => array());

            if($client[Type::AUTOINTIFICATION][Authorization::USER_AUTH])
            {
                $login = $client[Type::AUTOINTIFICATION][Authorization::USER_LOGIN];

                if($this->check_user_subscribe($login, $request->Group))
                {
                    $pubKey = $client[Type::HANDSHAKE][Handshake::PUBLIC_KEY];
                    $groupMessages = $this->get_group_messages($request->Group, $request->Date);
                    $messages = $this->convert_messages($pubKey, $groupMessages);

                    $responceData[GroupMessage::Messages] = $messages;
                }
            }

            return Data::encode($responceData);
        }

        public function execute(&$client, $request)
        {
            if($request->Type == Type::GROUP_MESSAGES)
                return $this->exec($client, $request);

            return parent::execute($client, $request);
        }
    }
?>
