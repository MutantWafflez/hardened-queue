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
        This roster file determines the set of users that are allowed to enter the queue, denoted by their ID (whatever format that 
        may be). This file MUST be in the format of <code>ID,FIRST_NAME,LAST_NAME</code> for each line, with NO extraneous 
        characters. Assuming this reading & parsing is successful, <b>hardened-queue</b>'s shell will then begin and 
        continuously prompt for a <u>command</u>. See section <u>Commands</u> for a list of all commands <b>hardened-queue</b>'s 
        shell accepts.
        </p>
    </div>
    <div style="margin-left: 63px;">
    <p>
    todo
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
            is needing to be restored or the user wishes to begin execution with a populated queue, if the <b>-b</b>
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
    <h3 style="margin-bottom: -15px !important; padding-bottom: 0px !important;">TODO</h3>
    <p style="margin-left: 63px;">
    todo
    </p>
</div>

<div>
    <div style="float: left">HARDENED-QUEUE v1.0</div>
    <div style="float: right">HARDENED-QUEUE v1.0</div>
    <div style="margin:  auto; width: 175px;">June 12th, 2024</div>
</div>