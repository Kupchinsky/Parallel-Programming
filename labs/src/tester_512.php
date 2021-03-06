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

	for ($i = 1000; $i <= 15000; $i += 1000)
	{
		for ($j = 2; $j <= 4; $j += 2)
		{
			$result = my_exec(
				escapeshellarg('..' . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR . 'run_silent_mpe.bat')  . ' hometask5_1_2.cpp ' . $j . ' ' . $i
			);

			if ($result['return'] != 0)
			{
				$data .= 'Program crashes!' . PHP_EOL . $result['stdout'] . PHP_EOL;
				break 2;
			}

			$result = preg_grep('/^elapsed time: (.*)$/', explode("\n", $result['stdout']));
			$data .= $i . ', ' . $j . ', ' . array_pop($result) . PHP_EOL;
		}
	}

	file_put_contents(__FILE__ . '.txt', $data);