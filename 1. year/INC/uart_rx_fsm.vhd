-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): xluptak00

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port(
       CLK : in std_logic;
       RST : in std_logic;
       DIN : in std_logic;
       CLK_CNT1 : in std_logic_vector(2 downto 0);
       CLK_CNT2 : in std_logic_vector(4 downto 0);
       BIT_CNT : in std_logic_vector(3 downto 0);
       VALID : out std_logic;
       READ_COUNT : out std_logic;
       START_COUNT : out std_logic

    );
end entity;


architecture behavioral of UART_RX_FSM is
type state_type is (WAIT_DIN, START, READ, STOP, RET);
signal state : state_type := WAIT_DIN;
begin

    VALID <= '1' when state = RET else '0';
    READ_COUNT <= '1' when state = READ else '0';
    START_COUNT <= '1' when state = START else '0';

    process (CLK, RST)
    begin
    if(RST = '1') then
        state <= WAIT_DIN;

    elsif rising_edge(CLK) then
        if (state = WAIT_DIN) then
            if (DIN = '0') then
                state <= START;
            end if;

        elsif(state = START) then
            if (CLK_CNT1 = "110") then
                state <= READ;
            end if;
    
        elsif(state = READ) then
            if(BIT_CNT = "1000") then
                state <= STOP;
            end if;

        elsif(state = STOP) then
            if(DIN = '1') then
                state <= RET;
            end if;

        elsif(state = RET) then
            state <= WAIT_DIN;
        end if;
    end if;
        
    end process;
    

end architecture;
