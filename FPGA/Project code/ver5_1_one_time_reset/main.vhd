----------------------------------------------------------------------------------
-- Company: SDU - Project group 2
-- Version: 3
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity main is
		generic(
				N:			Natural:= 11;
				MAX:		Natural:= 2048		-- Should be 2**N
				);
		port(
				Clk:				IN  STD_LOGIC;
				Hallsensor1A:	IN  STD_LOGIC;
		 		Hallsensor1B:	IN  STD_LOGIC;
				Hallsensor2A:	IN  STD_LOGIC;
				Hallsensor2B:	IN  STD_LOGIC;
				Hallsensor0:	IN  STD_LOGIC;
				Hallsensor1:	IN  STD_LOGIC;
				ENA:				OUT STD_LOGIC; 
				ENB:				OUT STD_LOGIC;
				IN1A:				OUT STD_LOGIC;
				IN2A:				OUT STD_LOGIC;
				IN1B:				OUT STD_LOGIC;
				IN2B:				OUT STD_LOGIC;
				-- SPI --
				SCLK:				IN  STD_LOGIC;
				SS:				IN  STD_LOGIC;
				MISO:				OUT STD_LOGIC;
				MOSI:				IN  STD_LOGIC;
				---------
				sw:				IN  STD_LOGIC_VECTOR(1 downto 0);
				an: 				OUT STD_LOGIC_VECTOR(3 downto 0);
				a_to_g:			OUT STD_LOGIC_VECTOR(6 downto 0)
				);   
end main; 
architecture Behavioral of main is


--- ALL THE SIGNALS SHOULD BE DECLARED HERE ---
signal direction_m1, direction_m2: 					STD_LOGIC;
signal s_Clk_1kHz:										STD_LOGIC;
signal SPI_dataIn, SPI_dataOut:						STD_LOGIC_VECTOR(15 downto 0);
signal PWM_channel:										STD_LOGIC_VECTOR(1 downto 0);
signal PWM_threshold:									STD_LOGIC_VECTOR(10 downto 0);
signal m1_tics_ones, m1_tics_tens:					STD_LOGIC_VECTOR(3 downto 0);
signal m1_tics_hundreds, m1_tics_thousands:		STD_LOGIC_VECTOR(3 downto 0);
signal m2_tics_ones, m2_tics_tens:					STD_LOGIC_VECTOR(3 downto 0);
signal m2_tics_hundreds, m2_tics_thousands:		STD_LOGIC_VECTOR(3 downto 0);
signal m1_count, m2_count:								STD_LOGIC_VECTOR(10 downto 0);
signal reset_m1: 											STD_LOGIC;
signal reset_m2: 											STD_LOGIC;
signal a_to_g_tmp1, a_to_g_tmp2:						STD_LOGIC_VECTOR(6 downto 0);
signal an_tmp1, an_tmp2:								STD_LOGIC_VECTOR(3 downto 0);
-----------------------------------------------

--- ALL THE COMPONENTS SHOULD BE DECLARED HERE ---
	component tic_counter 
		port(
				Clk:					IN  STD_LOGIC; 
				HallsensorA:		IN  STD_LOGIC;
			  	HallsensorB:		IN  STD_LOGIC;
				Reset:				IN  STD_LOGIC;
				count_direction:	OUT STD_LOGIC;
				s_ones:				OUT STD_LOGIC_VECTOR(3 downto 0);
				s_tens:				OUT STD_LOGIC_VECTOR(3 downto 0);
				s_hundreds:			OUT STD_LOGIC_VECTOR(3 downto 0);
				s_thousands:		OUT STD_LOGIC_VECTOR(3 downto 0);
				Count:				OUT STD_LOGIC_VECTOR(10 downto 0)
				);
	end component;
	
	component display_debug
		port(
				Clk:					IN  STD_LOGIC;
				Clk_1kHz:			IN  STD_LOGIC; 
				ones:					IN STD_LOGIC_VECTOR(3 downto 0);
				tens:					IN STD_LOGIC_VECTOR(3 downto 0);
				hundreds:			IN STD_LOGIC_VECTOR(3 downto 0);
				reset:				IN  STD_LOGIC;
				thousands:			IN STD_LOGIC_VECTOR(3 downto 0);
				an:					OUT STD_LOGIC_VECTOR(3 downto 0);
				a_to_g:				OUT STD_LOGIC_VECTOR(6 downto 0)
				);
	end component;
	
	component clk_scaler
		port(
				Clk:					IN  STD_LOGIC;
				Clk_1kHz:			OUT STD_LOGIC
				);
	end component;
	
	component PWM_generator
		generic(
				N:		 				Natural;
				MAX:					Natural
				);
		port(
				Clk:					IN  STD_LOGIC;
				Threshold:			IN  STD_LOGIC_VECTOR(N-1 downto 0);
				PWM_channel:		IN  STD_LOGIC_VECTOR(1 downto 0);
				PWM_IN1A:			OUT STD_LOGIC;
				PWM_IN2A:			OUT STD_LOGIC;
				PWM_IN1B:			OUT STD_LOGIC;
				PWM_IN2B:			OUT STD_LOGIC
				);
	end component;
	
	component input_decoder 
		port(
				clk:					IN  STD_LOGIC;	
				SPI_data:			IN  STD_LOGIC_VECTOR(15 downto 0);
				PWM_channel:		OUT STD_LOGIC_VECTOR(1 downto 0);
				PWM_threshold:		OUT STD_LOGIC_VECTOR(10 downto 0)	
				);
	end component;
	
	component decoder_reset is
		port(	
				clk:					IN  STD_LOGIC;
				Hallsensor:			IN  STD_LOGIC;
				Direction:			IN  STD_LOGIC;
				reset:				OUT STD_LOGIC
			);
	end component;
	
	component SPI is 
		port(
				Clk: 					IN  STD_LOGIC;										-- FPGAens Clk (rising)
			  -- SPI --
			  SCLK:	 				IN  STD_LOGIC;										-- Masters Clk (rising)
			  SS: 					IN  STD_LOGIC;										-- Slave Select - When high data is send (falling)
           MOSI: 					IN  STD_LOGIC;										-- Master Out Slave In	  
			  MISO: 					OUT STD_LOGIC;										-- Master In Slave Out
			  -- Dummy Register --
			  Data_out_from_FPGA:IN  STD_LOGIC_VECTOR (15 downto 0);			-- Data from register
			  Data_into_FPGA: 	OUT STD_LOGIC_VECTOR (15 downto 0)			-- Data to registe
			);
	end component;
	
	component output_decoder is 
		port(									
			  SS: 					IN  STD_LOGIC;										-- Slave Select - When high data is send (falling)
			  count_m1:				IN  STD_LOGIC_VECTOR(10 downto 0);
			  count_m2: 			IN  STD_LOGIC_VECTOR(10 downto 0);
			  DataOut: 				OUT STD_LOGIC_VECTOR (15 downto 0)
			  );
	end component;

--------------------------------------------------
begin

--- ALL THE PORTMAPPING SHOULD BE DONE HERE ---
TC1:	tic_counter port map(
		Clk => Clk,
		HallsensorA => Hallsensor1A,
		HallsensorB => Hallsensor1B,
		Reset => reset_m1,
		s_ones => m1_tics_ones,
		s_tens => m1_tics_tens,
		s_hundreds => m1_tics_hundreds,
		s_thousands => m1_tics_thousands,
		count_direction => direction_m1,
		count => m1_count
		);
		
TC2:	tic_counter port map(
		Clk => Clk,
		HallsensorA => Hallsensor2A,
		HallsensorB => Hallsensor2B,
		Reset => reset_m2,
		s_ones => m2_tics_ones,
		s_tens => m2_tics_tens,
		s_hundreds => m2_tics_hundreds,
		s_thousands => m2_tics_thousands,
		count_direction => direction_m2,
		count => m2_count
		);
		 
CS:	clk_scaler port map(
		Clk => Clk,
		Clk_1kHz => s_Clk_1kHz
		);
				
DD1:	display_debug port map(
		Clk => Clk,
		Clk_1kHz => s_Clk_1kHz,
		an => an_tmp1,
		a_to_g => a_to_g_tmp1,
		reset => reset_m1,
		ones => m1_tics_ones,
		tens => m1_tics_tens,
		hundreds => m1_tics_hundreds,
		thousands => m1_tics_thousands
		);	
		
DD2:	display_debug port map(
		Clk => Clk,
		Clk_1kHz => s_Clk_1kHz,
		an => an_tmp2,
		a_to_g => a_to_g_tmp2,
		reset => reset_m2,
		ones => m2_tics_ones,
		tens => m2_tics_tens,
		hundreds => m2_tics_hundreds,
		thousands => m2_tics_thousands
		);			
		
PWM:	PWM_generator 
		generic map(
		N => N,
		MAX => MAX
		)
		port map(
		clk => clk,
		Threshold => PWM_threshold,
		PWM_channel => PWM_channel,
		PWM_IN1A => IN1A,
		PWM_IN2A => IN2A,
		PWM_IN1B => IN1B,
		PWM_IN2B => IN2B
		);
		
ID:	input_decoder port map(
		clk => clk,
		SPI_data => SPI_dataOut,
		PWM_channel => PWM_channel,
		PWM_threshold => PWM_threshold
		);

res_1: decoder_reset port map(
		clk => clk,
		Hallsensor => Hallsensor0,
		direction => direction_m1,
		reset => reset_m1
		);
		
res_2: decoder_reset port map(
		clk => clk,
 		Hallsensor => Hallsensor1,
		direction => direction_m2,
 		reset => reset_m2
 		);
		
SPI1:	SPI port map(
		clk => clk,
		SCLK => SCLK,
		SS => SS,
		MOSI => MOSI,
		MISO => MISO,
		Data_out_from_FPGA => SPI_dataIn,
		Data_into_FPGA => SPI_dataOut
		);
		
OD:	output_decoder port map(
		SS => SS,
		count_m1 => m1_count,
		count_m2 => m2_count,
		Dataout => SPI_dataIn
		);
-------------------------------------------------		

--- ALL THE CODE SHOULD BE HERE ---
	process(clk)
		begin
	if rising_edge(clk) then
	
		if sw(1) = '0' then 
			an <= an_tmp2;  
			a_to_g <= a_to_g_tmp2;
		else
			an <= an_tmp1;
			a_to_g <= a_to_g_tmp1;
		end if;
	 
		if sw(0) = '0' then
			ENA <= '0';
			ENB <= '0';
		else 
			ENA <= '1';
			ENB <= '1';
		end if;
	end if;
end process;
-----------------------------------
end Behavioral;

