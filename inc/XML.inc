'##############################################################################
'#
'#  This is one of the include files for << RFO-BASIC! Quick APK >>
'#
'#  "xml.inc" contains routines to parse an eXtended Markup Language (XML) file
'#
'#  An XML file follows the following structure:
'#        <tag param="inline_value"> content </tag>
'#    or  <tag param='inline_value' />
'#
'##############################################################################

'------------------------------------------------------------------------------
FUNCTION XmlContent(buffer AS STRING, tag AS STRING) AS STRING
' Retrieve 'content' from   <tag param="inline_value"> content </tag>
' Hint: you can specify a 'tag' to be searched for, or a 'tag param'
    LOCAL i, j AS LONG
    LOCAL ct AS STRING

    i = INSTR(LCASE$(buffer), "<" + LCASE$(tag))
    IF i = 0 THEN EXIT FUNCTION ' opening tag not found

    IF INSTR(tag, $SPC) = 0 THEN ct = "</" + tag  + ">" ELSE ct = "</" + LEFT$(tag, INSTR(tag, $SPC) - 1) + ">" ' closing tag

    i += LEN(tag) + 1
    j = INSTR(i, buffer, "/>")
    i = INSTR(i, buffer, ">")
    IF i = j+1 THEN EXIT FUNCTION ' <tag param='inline_value' />   --> tag has no content

    i += 1
    j = INSTR(i, LCASE$(buffer), LCASE$(ct))
    IF j = 0 THEN EXIT FUNCTION ' closing tag not found

    FUNCTION = MID$(buffer, i, j-i)

END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ReplaceXmlContentWith(BYREF buffer AS STRING, tag AS STRING, newcontent AS STRING)
' Replace 'content' with 'newcontent' in   <tag param="inline_value"> content </tag>
' /or/ transform a   <tag param='inline_value' />   into a   <tag param='inline_value' > newcontent </tag>
' Hint: you can specify a 'tag' to be searched for, or a 'tag param'
    LOCAL i, j AS LONG
    LOCAL ct AS STRING

    i = INSTR(LCASE$(buffer), "<" + LCASE$(tag))
    IF i = 0 THEN EXIT SUB ' opening tag not found

    IF INSTR(tag, $SPC) = 0 THEN ct = "</" + tag + ">" ELSE ct = "</" + LEFT$(tag, INSTR(tag, $SPC) - 1) + ">" ' closing tag

    i += LEN(tag) + 1
    j = INSTR(i, buffer, "/>")
    i = INSTR(i, buffer, ">")

    IF i = j+1 THEN            ' <tag param='inline_value' />   --> tag found but has no content initialy  --> create it
        buffer = LEFT$(buffer, j-1) + ">" + newcontent + ct + MID$(buffer, j+2)
    ELSE
        i += 1
        j = INSTR(i, LCASE$(buffer), LCASE$(ct))
        IF j <> 0 THEN buffer = LEFT$(buffer, i-1) + newcontent + MID$(buffer, j) ' closing tag found --> replace content
    END IF

END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION InlineContent(buffer AS STRING, param AS STRING) AS STRING
' Retrieve 'inline_value' from   <tag param="inline_value"> content </tag>   or from   <tag param='inline_value' />
    LOCAL i, j AS LONG
    LOCAL t AS STRING

    t = "=" + $DQ
    i = INSTR(LCASE$(buffer), LCASE$(param) + t)              ' double quote, no space around equal sign
    IF i = 0 THEN
        t = "='"
        i = INSTR(LCASE$(buffer), LCASE$(param) + t)          ' simple quote, no space around equal sign
        IF i = 0 THEN
            t = " = " + $DQ
            i = INSTR(LCASE$(buffer), LCASE$(param) + t)      ' double quote, spaces around equal sign
            IF i = 0 THEN
                t = " = '"
                i = INSTR(LCASE$(buffer), LCASE$(param) + t)  ' simple quote, spaces around equal sign
            END IF
        END IF
    END IF
    IF i = 0 THEN EXIT FUNCTION ' param not found

    i += LEN(param + t)
    j = INSTR(i, LCASE$(buffer), RIGHT$(t, 1))
    IF j = 0 THEN EXIT FUNCTION ' closing quote not found

    FUNCTION = MID$(buffer, i, j-i)

END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ReplaceInlineContentWith(BYREF buffer AS STRING, param AS STRING, newvalue AS STRING)
' Replace 'inline_value' with 'newvalue' in   <tag param="inline_value"> content </tag>
    LOCAL i, j AS LONG
    LOCAL t AS STRING

    t = "=" + $DQ
    i = INSTR(LCASE$(buffer), LCASE$(param) + t)              ' double quote, no space around equal sign
    IF i = 0 THEN
        t = "='"
        i = INSTR(LCASE$(buffer), LCASE$(param) + t)          ' simple quote, no space around equal sign
        IF i = 0 THEN
            t = " = " + $DQ
            i = INSTR(LCASE$(buffer), LCASE$(param) + t)      ' double quote, spaces around equal sign
            IF i = 0 THEN
                t = " = '"
                i = INSTR(LCASE$(buffer), LCASE$(param) + t)  ' simple quote, spaces around equal sign
            END IF
        END IF
    END IF
    IF i = 0 THEN EXIT SUB ' param not found

    i += LEN(param + t)
    j = INSTR(i, LCASE$(buffer), RIGHT$(t, 1))
    IF j = 0 THEN EXIT SUB ' closing quote not found

    buffer = LEFT$(buffer, i-1) + newvalue + MID$(buffer, j)

END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ReplaceInlineContentAfterTagWith(BYREF buffer AS STRING, tag AS STRING, param AS STRING, newvalue AS STRING)
' Replace 'inline_value' with 'newvalue' in   <tag param="inline_value"> content </tag>
    LOCAL i, j, k AS LONG
    LOCAL t AS STRING

    k = INSTR(LCASE$(buffer), "<" + LCASE$(tag))
    IF k = 0 THEN EXIT SUB ' tag not found

    t = "=" + $DQ
    i = INSTR(k, LCASE$(buffer), LCASE$(param) + t)              ' double quote, no space around equal sign
    IF i = 0 THEN
        t = "='"
        i = INSTR(k, LCASE$(buffer), LCASE$(param) + t)          ' simple quote, no space around equal sign
        IF i = 0 THEN
            t = " = " + $DQ
            i = INSTR(k, LCASE$(buffer), LCASE$(param) + t)      ' double quote, spaces around equal sign
            IF i = 0 THEN
                t = " = '"
                i = INSTR(k, LCASE$(buffer), LCASE$(param) + t)  ' simple quote, spaces around equal sign
            END IF
        END IF
    END IF
    IF i = 0 THEN EXIT SUB ' param not found

    j = INSTR(k, buffer, ">")
    IF j < i THEN EXIT SUB ' param outside of scope of tag (e.g. <tag ... /> <other_incorrect_tag  param_we_look_for...>

    i += LEN(param + t)
    j = INSTR(i, LCASE$(buffer), RIGHT$(t, 1))
    IF j = 0 THEN EXIT SUB ' closing quote not found

    buffer = LEFT$(buffer, i-1) + newvalue + MID$(buffer, j)

END SUB
'------------------------------------------------------------------------------
