#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

/* Working mode for BH1750 */
#define BH1750_ONE_TIME_H_RES_MODE  0x20

static uint16_t prev_result = 0;

struct bh1750_t {
  struct i2c_client *client;
  struct miscdevice bh1750_misc;
  struct mutex lock;
};

/* =============== READ SENSORS =============== */
static int bh1750_get_sensor_value(struct bh1750_t *bh1750, uint16_t *result)
{
  int ret;
  char sensor_mode = BH1750_ONE_TIME_H_RES_MODE;

  /* Protect resource */
  mutex_lock(&bh1750->lock);

  /* Send cmd to set sensor mode and wait for measuring  */
  ret = i2c_master_send(bh1750->client, &sensor_mode, 1);
  if (ret < 0) goto out;
  usleep_range(120000, 180000);

  /* Read measurement result in Big-endian */
  ret = i2c_master_recv(bh1750->client, (char *)result, 2);
  if (ret < 0) goto out;

  /* Convert to PC storing-type */
  *result = be16_to_cpu(*result);

  /* Result filter */
  if ((*result - prev_result) > 1500)
    *result = prev_result;
  else prev_result = *result;

out:
  mutex_unlock(&bh1750->lock);
  return (ret >= 0) ? 0 : -EFAULT;
}

/* =============== USER-SPACE READ DEVICE-FILE =============== */
static ssize_t bh1750_read_file(struct file *file, char __user *user_buf,
                                size_t count, loff_t *ppos)
{
  struct bh1750_t *bh1750;
  uint16_t result;
  char buffer[6];
  int ret;

  bh1750 = container_of(file->private_data, struct bh1750_t, bh1750_misc);
  ret = bh1750_get_sensor_value(bh1750, &result);
  if (ret == -EFAULT) return ret;

  /* Convert value to string and send to user-space */
  // printk(KERN_INFO "%d\n", result);
  sprintf(buffer, "%d", result);
  buffer[5] = '\n';

  if (*ppos == 0)
  {
    if (copy_to_user(user_buf, buffer, sizeof(buffer)))
      return -EFAULT;
    
    *ppos += 1;
    return sizeof(buffer);
  }

  return 0;
}

static const struct file_operations bh1750_fops = {
  .owner = THIS_MODULE,
  .read = bh1750_read_file,
};

/* =============== PLATFORM DEVICE =============== */
static int bh1750_probe(struct i2c_client *client,
                        const struct i2c_device_id *id)
{
  struct bh1750_t *bh1750;
  struct device *dev = &client->dev;

  /* Allocate new private structure */
  bh1750 = devm_kzalloc(dev, sizeof(struct bh1750_t), GFP_KERNEL);
  if (!bh1750)
    return -ENOMEM;

  /* Store device-structure to context */
  i2c_set_clientdata(client, bh1750);
  bh1750->client = client;

  mutex_init(&bh1750->lock);

  /* Setup for miscdevice */
  bh1750->bh1750_misc.name = "bh1750";
  bh1750->bh1750_misc.minor = MISC_DYNAMIC_MINOR;
  bh1750->bh1750_misc.fops = &bh1750_fops;

  return misc_register(&bh1750->bh1750_misc);
}

static int bh1750_remove(struct i2c_client *client)
{
  struct bh1750_t *bh1750;

  /* Get device-structure from context */
  bh1750 = i2c_get_clientdata(client); 
  
  misc_deregister(&bh1750->bh1750_misc);
  
  return 0;
}

static const struct i2c_device_id bh1750_id[] = {
  { "bh1750", 0 },
  { },
};
MODULE_DEVICE_TABLE(i2c, bh1750_id);

/* Matching device and driver in DeviceTree */
static const struct of_device_id bh1750_match[] = {
  { .compatible = "rohm,bh1750" },
  { }
};
MODULE_DEVICE_TABLE(of, bh1750_match);

static struct i2c_driver bh1750_i2c_driver = {
	.driver = {
		.name = "bh1750",
		.owner = THIS_MODULE,
    .of_match_table = bh1750_match,
	},
	.probe = bh1750_probe,
	.remove = bh1750_remove,
	.id_table = bh1750_id,
};
module_i2c_driver(bh1750_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Binh Nguyen <binhnt1612@gmail.com>");
MODULE_DESCRIPTION("I2C driver for light sensor");