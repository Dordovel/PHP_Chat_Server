<?php

    class Data
    {
        static private function encode_data($data)
        {
            foreach ($data as &$value)
            {
                if (is_array($value))
                {
                    $value = Data::encode_data($value);
                }
                else
                {
                    $value = base64_encode($value);
                }
            }

            return $data;
        }

        static private function decode_array($data)
        {
            foreach ($data as &$value)
            {
                $value = Data::decode_data($value);
            }

            return $data;
        }

        static private function decode_data($data)
        {
            $fields = array_keys(get_object_vars($data));
            foreach ($fields as $field)
            {
                if(is_array($data->{$field}))
                {
                    $data->{$field} = Data::decode_array($data->{$field});
                }
                else
                {
                    $data->{$field} = base64_decode($data->{$field});
                }
            }

            return $data;
        }

        static public function encode(array $data)
        {
            $data = Data::encode_data($data);
            return json_encode($data);
        }

        static public function decode($data)
        {
            $data = json_decode($data);

            $data = Data::decode_data($data);

            return $data;
        }
    }
?>