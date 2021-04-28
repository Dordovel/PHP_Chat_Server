<?php

	abstract class RequestHandler
	{
		const Status = 'Status';
		protected $_handle;

		public function next(RequestHandler $handle)
		{
			$this->_handle = $handle;

			return $this->_handle;
		}

		/**
		 * @param $client
		 * @param $request
		 * @return Data
		 */
		public function execute(&$client, $request)
		{
			if(isset($this->_handle))
				return $this->_handle->execute($client, $request);
			return Data::encode(array('Type' => $request->Type,
				RequestHandler::Status => ResponceStatus::FAILD));
		}
	}

?>
