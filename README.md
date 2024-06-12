<h2>
    <div style="float: left">HARDENED-QUEUE(1)</div>
    <div style="float: right">HARDENED-QUEUE(1)</div>
    <div style="margin: 0 auto; width: 282px;">Queue Manager's Manual</div>
</h2>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">NAME</h3>
    <p style="margin-left: 63px; margin-top"><b>hardened-queue</b> - protected shell for a queue of people</p>
</div>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">SYNOPSIS</h3>
    <p style="margin-left: 63px; margin-top"><b>hardened-queue</b> [<b>-b</b> backup-file] [<b>-p</b> prompt]</p>
</div>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">DESCRIPTION</h3>
    <div style="margin-left: 63px;">
        <p>
        <b>hardened-queue</b> is a long running program ran in a shell, protecting an internal queue of people that grows and shrinks
        through commands in the shell. It is designed to only allow a certain set of individuals, defined by the roster file, 
        to enter the queue. Only administrators with the admin password may remove (or "pop") people from the front of the queue.
        </p>
        <p>
        Upon startup of the queue, it will look for and attempt to read the <code>roster.csv</code> file in its working directory.
        This roster file determines the set of people that are allowed to enter the queue, denoted by their ID (whatever format that 
        may be). This file MUST be in the format of <code>ID,FIRST_NAME,LAST_NAME</code> for each line, with NO extraneous 
        characters. Assuming this reading & parsing is successful, <b>hardened-queue</b>'s shell will then begin and 
        continuously prompt for a <u>command</u>. See section <u>Commands</u> for a list of all commands <b>hardened-queue</b>'s 
        shell accepts.
        </p>
        <p>
        <b>hardened-queue</b>, as its name implies, is a hardened version of queue, named as such due to its resilience against
        <em>novice</em> sabotage. This is done with two main mechanisms: disabling of <code>SIGINT</code> (<code>CTRL+C</code>)
        and <code>SIGTSTP</code>(<code>CTRL+Z</code>), as well as queue backup mechanism in case of successful sabotage. See
        section <u>Security Notes</u> for why the "protections" are so limited.
        </p>
        <p>
        Every minute, <b>hardened-queue</b> will produce a backup file containing the state of the internal queue, named
        <code>queue_backup.csv</code>. It is in the format of <code>ID,FIRST_NAME,LAST_NAME</code>. This can be forced
        at will by using the <code>backup</code> command (see <u>Commands</u> section). It will also store a log file, named
        <code>queue_log.log</code>, which logs when <b>hardened-queue</b> starts, ends, and when the internal queue's
        state changes (i.e. is added to or popped from).
        </p>
    </div>
    <div style="margin-left: 63px;">
        <p>
        The options for <b>hardened-queue</b> are as follows:
        </p>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>-b</b> <u>backup-file</u>
            </p>
            <p style="margin-left: 63px;">
            Normally, when <b>hardened-queue</b> begins execution, the internal queue will be empty. However, if a backup
            is needing to be restored or the administrator wishes to begin execution with a populated queue, if the <b>-b</b>
            option is specified, the internal queue will be populated with the entries of the file denoted by the path
            <u>backup-file</u>. <u>backup-file</u> should have the same format as the roster file, i.e. 
            <code>ID,FIRST_NAME,LAST_NAME</code>. This option makes the assumption that each entry in <u>backup-file</u>
            is listed on the roster.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>-p</b> <u>prompt</u>
            </p>
            <p style="margin-left: 63px;">
            Changes the prompt given to the user when requesting for a command. If option <b>-p</b> is not specified, the
            prompt will default to <code>hardened-queue></code>.
            </p>
        </div>
    </div>
</div>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">COMMANDS</h3>
    <div style="margin-left: 63px;">
        <p>
        After successful initialization, <b>hardened-queue</b> will then continuously prompt the user for commands.
        The list of valid commands are as follows:
        </p>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>add</b> <u>id-string</u>
            </p>
            <p style="margin-left: 63px;">
            Adds to the tail of the internal queue the person denoted by <u>id-string</u>. If no person exists on the roster with 
            the id of <u>id-string</u> OR the person does exist but is already on the queue, this command will fail. Does
            not require administrator password.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>pop</b>
            </p>
            <p style="margin-left: 63px;">
            Removes the person from the front of the queue, and prints their name. Has no effect if the queue is empty.
            Requires administrator password.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>view</b>
            </p>
            <p style="margin-left: 63px;">
            Prints the current state of the internal queue, i.e. all people currently in the internal queue, in order. 
            Does not require administrator password.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>clear</b>
            </p>
            <p style="margin-left: 63px;">
            Clears the terminal screen. Requires administrator password.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>backup</b>
            </p>
            <p style="margin-left: 63px;">
            Forces a backup of the internal queue. Does not require administrator password.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>lock</b>
            </p>
            <p style="margin-left: 63px;">
            Locks/unlocks the internal queue. A locked internal queue prevents any new adds with the <b>add</b> command. 
            Administrators can <b>pop</b> as usual. Requires administrator password.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>help</b>
            </p>
            <p style="margin-left: 63px;">
            Prints out list of all commands and a short description. Does not require administrator password.
            </p>
        </div>
        <div>
            <p style="margin-bottom: -20px !important; padding-bottom: 0px !important;">
            <b>quit</b> | <b>exit</b>
            </p>
            <p style="margin-left: 63px;">
            Backs up the internal queue and and then exits the shell. Requires administrator password.
            </p>
        </div>
    </div>
</div>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">INSTALLATION & USAGE</h3>
    <div style="margin-left: 63px;">
        <p>
        <b>hardened-queue</b> currently only has releases on x86 Linux and Windows. To use <b>hardened-queue</b>, simply
        download the respective executable from the <u>Releases</u> page (on github). Remember, this is a program that needs
        to be ran <em>in a shell</em>, it is not its own stand-alone program - also, make sure your necessary files are in 
        the same working directory as the executable (namely your roster file and backup file, if applicable).
        </p>
    </div>
</div>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">SECURITY NOTES</h3>
    <div style="margin-left: 63px;">
        <p>
        As stated briefly in the <u>Description</u> section, <b>hardened-queue</b> has been designed to only protect
        against <em>novice</em> sabotage. The reason for this is two-fold: firstly, full lockdown on any arbitrary system
        is a rather complex thing to do, especially for a small shell program like this one. Secondly: it's a bit out of
        scope for me (the author) at the moment (and probably indefinitely). That being said, if someone wants to undertake
        the endeavor to <em>really</em> make this a super secure shell program - be my guest! But, for the time being, someone
        could very easily just <code>ALT-F4</code> or just close the terminal straight up.
        </p>
        <p>
        Also, the administrator password is currently stored in plain-text, inside the code-base. Yeah, not very secure.
        But, for the context of why this application was created, it's good enough. I am open to making it more secure, 
        if other people need it to be as such.
        </p>
    </div>
</div>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">AUTHOR</h3>
    <div style="margin-left: 63px;">
        <p>
        Created by <a href="https://github.com/MutantWafflez">MutantWafflez</a>.
        </p>
    </div>
</div>

<div>
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">COPYRIGHT</h3>
    <div style="margin-left: 63px;">
        <p>
        Check the <b>LICENSE</b> file provided with the source code. For quick reference,0 it is the GPL-3.0 license.
        </p>
    </div>
</div>

<div>
    <div style="float: left">HARDENED-QUEUE v1.0</div>
    <div style="float: right">HARDENED-QUEUE v1.0</div>
    <div style="margin: 0 auto; width: 94px;">June 12th, 2024</div>
</div>