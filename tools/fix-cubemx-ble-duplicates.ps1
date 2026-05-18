$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$customStmPath = Join-Path $repoRoot 'STM32_WPAN\App\custom_stm.c'
$customAppPath = Join-Path $repoRoot 'STM32_WPAN\App\custom_app.c'

function Remove-SecondExactBlock {
    param(
        [Parameter(Mandatory = $true)] [string] $Text,
        [Parameter(Mandatory = $true)] [string] $Block
    )

    $first = $Text.IndexOf($Block)
    if ($first -lt 0) { return $Text }

    $second = $Text.IndexOf($Block, $first + $Block.Length)
    if ($second -lt 0) { return $Text }

    return $Text.Remove($second, $Block.Length)
}

$customStm = Get-Content -LiteralPath $customStmPath -Raw
$customStm = [regex]::Replace(
    $customStm,
    '(?ms)(uint16_t\s+SizeOrientation_Command\s*=\s*\d+;\r?\nuint16_t\s+SizeServo_Feedback\s*=\s*\d+;\r?\n)\1',
    '$1'
)
Set-Content -LiteralPath $customStmPath -Value $customStm -NoNewline

$customApp = Get-Content -LiteralPath $customAppPath -Raw
$prototypeBlock = @"
/* Orientation */
static void Custom_Servo_feedback_Update_Char(void);
static void Custom_Servo_feedback_Send_Notification(void);
"@
$customApp = Remove-SecondExactBlock -Text $customApp -Block $prototypeBlock

$functionBlock = @"
/* Orientation */
__USED void Custom_Servo_feedback_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Servo_feedback_UC_1*/

  /* USER CODE END Servo_feedback_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SERVO_FEEDBACK, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Servo_feedback_UC_Last*/

  /* USER CODE END Servo_feedback_UC_Last*/
  return;
}

void Custom_Servo_feedback_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Servo_feedback_NS_1*/

  /* USER CODE END Servo_feedback_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SERVO_FEEDBACK, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Servo_feedback_NS_Last*/

  /* USER CODE END Servo_feedback_NS_Last*/

  return;
}

"@
$customApp = Remove-SecondExactBlock -Text $customApp -Block $functionBlock
$customApp = $customApp.Replace('static Custom_App_Context_t Custom_App_Context;', 'static Custom_App_Context_t Custom_App_Context __attribute__((unused));')
$customApp = $customApp.Replace('void Custom_Servo_feedback_Send_Notification(void) /* Property Notification */', '__attribute__((unused)) void Custom_Servo_feedback_Send_Notification(void) /* Property Notification */')
Set-Content -LiteralPath $customAppPath -Value $customApp -NoNewline

Write-Host 'Removed duplicate CubeMX BLE stubs when present.'
