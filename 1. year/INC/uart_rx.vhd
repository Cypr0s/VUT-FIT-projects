-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): xluptak00

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0):= "00000000";
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
signal CLK_CNT1     : std_logic_vector(2 downto 0) := "000";
signal CLK_CNT2     : std_logic_vector(4 downto 0) := "00000";
signal BIT_CNT      : std_logic_vector(3 downto 0) := "0000";
signal START_COUNT  : std_logic;
signal READ_COUNT   : std_logic;
signal VALID        : std_logic;
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        CLK_CNT1 => CLK_CNT1,
        CLK_CNT2 => CLK_CNT2,
        BIT_CNT => BIT_CNT,
        START_COUNT => START_COUNT,
        READ_COUNT => READ_COUNT,
        VALID => VALID
    );
    process (CLK, RST) begin
        if(RST = '1') then
            CLK_CNT1 <= "000";
            CLK_CNT2 <= "00000";
            BIT_CNT <= "0000";
        
        elsif rising_edge(CLK) then
            
            if(START_COUNT = '1') then
                CLK_CNT1 <= CLK_CNT1 + 1;
            else
                CLK_CNT1 <= "000";
            end if;

            if(READ_COUNT = '1') then
                CLK_CNT2 <= CLK_CNT2 + 1;
                if(CLK_CNT2 = "10000") then
                    if (BIT_CNT = "0000") then
                        DOUT(0) <= DIN;
                    elsif(BIT_CNT = "0001") then
                        DOUT(1) <= DIN;
                    elsif(BIT_CNT = "0010") then
                        DOUT(2) <= DIN;
                    elsif(BIT_CNT = "0011") then
                        DOUT(3) <= DIN;
                    elsif(BIT_CNT = "0100") then
                        DOUT(4) <= DIN;
                    elsif(BIT_CNT = "0101") then
                        DOUT(5) <= DIN;
                    elsif(BIT_CNT = "0110") then
                        DOUT(6) <= DIN;
                    elsif(BIT_CNT = "0111") then
                        DOUT(7) <= DIN;
                    else
                        null;
                    end if;
                    BIT_CNT <= BIT_CNT + 1;
                    CLK_CNT2 <= "00000";
                end if;
            else
                BIT_CNT <= "0000";
                CLK_CNT2 <= "00000";
            end if;
                        
        end if;
    end process;
    DOUT_VLD <= VALID;
end architecture;
