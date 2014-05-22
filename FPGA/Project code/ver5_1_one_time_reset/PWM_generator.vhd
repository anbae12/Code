
--------------------------------------------------------
--
--	This component generates PWM to 4 channels. As an
-- input it gets a PWM channel and a threshold. It 
-- updates this channel with the threshold. It always 
-- generates PWM according to the threshold for the 4
-- channels.
--
--------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;

entity PWM_generator is
	generic(
				N: 			Natural;
				MAX:			Natural
			);
	port(
			Clk:				IN  STD_LOGIC;
			Threshold:		IN  STD_LOGIC_VECTOR(N-1 downto 0);	
			PWM_channel:	IN  STD_LOGIC_VECTOR(1 downto 0);
			PWM_IN1A:		OUT STD_LOGIC;
			PWM_IN2A:		OUT STD_LOGIC;
			PWM_IN1B:		OUT STD_LOGIC;
			PWM_IN2B:		OUT STD_LOGIC
			);
end PWM_generator;

architecture Behavioral of PWM_generator is
	signal PWM_count:																			STD_LOGIC_VECTOR(N-1 downto 0);
	signal IN1A_threshold, IN2A_threshold, IN1B_threshold, IN2B_threshold: 	STD_LOGIC_VECTOR(N-1 downto 0);

begin

--- THIS PROCESS SHOULD DETERMINE WHICH PWM-CHANNEL SHOULD BE UPDATED ---
process(Clk, PWM_channel, Threshold)
begin
	if rising_edge(clk) then		
		case PWM_channel is
			when "00" => 
				IN1A_threshold <= Threshold;
				IN2A_threshold <= (others =>'0');
			when "01" => 
				IN1A_threshold <= (others =>'0');
				IN2A_threshold <= Threshold;
			when "10" =>
				IN1B_threshold <= Threshold;
				IN2B_threshold <= (others => '0');
			when "11" =>
				IN1B_threshold <= (others => '0');
				IN2B_threshold <= Threshold;
			when others =>
				null;
		end case;
	end if;
end process;
--------------------------------------------------------------------------

--- THIS PROCESS SHOULD INCREMENT THE PWM_count EACH CLOCK CYCLE ---
process(Clk, Threshold, PWM_count)
begin
	if rising_edge(clk) then
		if PWM_count < MAX - 1 then
			PWM_count <= PWM_count + 1;
		else
			PWM_count <= (others => '0');
		end if;
	end if;
end process;
---------------------------------------------------------------------		


process(Clk, IN1A_threshold, IN2A_threshold, IN1B_threshold, IN2B_threshold)
begin
	if rising_edge(clk) then
	
		if (PWM_count < IN1A_threshold ) then --or PWM_count = IN1A_threshold) then
			PWM_IN1A <= '1';
		else 
			PWM_IN1A <= '0';
		end if;
		
		if (PWM_count < IN2A_threshold) then --or PWM_count = IN2A_threshold) then
			PWM_IN2A <= '1';
		else 
			PWM_IN2A <= '0';
		end if;
		
		if (PWM_count < IN1B_threshold) then -- or PWM_count = IN1B_threshold) then
			PWM_IN1B <= '1';
		else 
			PWM_IN1B <= '0';
		end if;
		
		if (PWM_count < IN2B_threshold) then -- or PWM_count = IN2B_threshold) then
			PWM_IN2B <= '1';
		else 
			PWM_IN2B <= '0';
		end if;
		
	end if;
end process;

end Behavioral;

