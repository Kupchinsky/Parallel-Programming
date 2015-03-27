<?
	function my_exec($cmd, $input = '')
	{
		//die($cmd);
		$proc = proc_open($cmd, array( 0 => array('pipe', 'r'), 1 => array('pipe', 'w'), 2 => array('pipe', 'w')), $pipes);

		fwrite($pipes[0], $input);
		fclose($pipes[0]);

		$stdout = stream_get_contents($pipes[1]);
		fclose($pipes[1]);

		$stderr = stream_get_contents($pipes[2]);
		fclose($pipes[2]);

		$rtn = proc_close($proc);

		return array('stdout' => $stdout, 'stderr' => $stderr, 'return' => $rtn);
	}

	chdir(dirname(__FILE__));

	$data = '';

	for ($i = 3; $i <= 9; $i++)
	{
		for ($j = 1; $j <= 3; $j++)
		{
			$result = my_exec(
				escapeshellarg('..' . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR . 'run_silent.bat')  . ' hometask1_4.cpp 1 ' . $i
			);

			$result = preg_grep('/^Result: (.*)$/', explode("\n", $result['stdout']));
			$data .= '10^' . $i . ': ' . array_pop($result) . PHP_EOL;
		}
		$data .= PHP_EOL;
	}

	file_put_contents(__FILE__ . '.txt', $data);