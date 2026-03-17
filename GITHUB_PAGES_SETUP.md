# GitHub Pages Setup Guide

## Quick Setup (Manual - Recommended)

Since the GitHub Actions workflow requires a token with `workflow` scope, here's the simpler manual approach:

### Step 1: Enable GitHub Pages

1. Go to your repository: https://github.com/Adhz33/Off-Highway-Guardian
2. Click **Settings** (top right)
3. Click **Pages** (left sidebar)
4. Under "Build and deployment":
   - **Source:** Select "Deploy from a branch"
   - **Branch:** Select `main`
   - **Folder:** Select `/web-frontend`
5. Click **Save**

### Step 2: Wait for Deployment

GitHub will automatically deploy your site. This takes 1-2 minutes.

### Step 3: Access Your Site

Your site will be live at:
```
https://adhz33.github.io/Off-Highway-Guardian/
```

That's it! No GitHub Actions needed.

---

## Alternative: GitHub Actions (Advanced)

If you want automatic deployments with GitHub Actions:

### Step 1: Create New Token with Workflow Scope

1. Go to: https://github.com/settings/tokens
2. Click "Generate new token (classic)"
3. Select scopes:
   - ✅ `repo` (full control)
   - ✅ `workflow` (update workflows)
4. Generate and copy the token
5. **Keep it secure!**

### Step 2: Push the Workflow

The workflow file is already created at `.github/workflows/deploy.yml`

Push it with your new token:
```bash
cd ~/Desktop/off-highway-guardian
git push https://YOUR_USERNAME:YOUR_NEW_TOKEN@github.com/Adhz33/Off-Highway-Guardian.git main
```

### Step 3: Enable GitHub Pages for Actions

1. Go to: https://github.com/Adhz33/Off-Highway-Guardian/settings/pages
2. Under "Build and deployment":
   - **Source:** Select "GitHub Actions"
3. The workflow will run automatically on every push

---

## Verify Deployment

Once deployed, your site will be available at:

**🌐 https://adhz33.github.io/Off-Highway-Guardian/**

### What's Deployed

- Interactive web dashboard
- Live accident detection simulator
- Complete documentation viewer
- System architecture showcase
- Team information
- Fully responsive design

---

## Troubleshooting

### Site Not Loading?

1. Check deployment status:
   - Go to: https://github.com/Adhz33/Off-Highway-Guardian/actions
   - Look for green checkmark ✅

2. Check Pages settings:
   - Go to: https://github.com/Adhz33/Off-Highway-Guardian/settings/pages
   - Verify source is set correctly

3. Wait a few minutes:
   - First deployment can take 2-5 minutes
   - Subsequent deployments are faster

### 404 Error?

Make sure the branch and folder are set correctly:
- Branch: `main`
- Folder: `/web-frontend`

### Changes Not Showing?

1. Clear browser cache (Ctrl+Shift+R or Cmd+Shift+R)
2. Wait 1-2 minutes for GitHub to rebuild
3. Check if the latest commit is deployed

---

## Custom Domain (Optional)

To use a custom domain like `guardian.yourdomain.com`:

1. Add a `CNAME` file to `web-frontend/`:
   ```
   guardian.yourdomain.com
   ```

2. Configure DNS:
   - Add CNAME record pointing to: `adhz33.github.io`

3. Update GitHub Pages settings with your custom domain

---

## Local Development

To test locally before pushing:

```bash
cd ~/Desktop/off-highway-guardian/web-frontend
python3 -m http.server 3000
```

Open: http://localhost:3000

---

## Recommended: Manual Setup

For this project, the **manual setup** (Step 1 above) is recommended because:

✅ No token management needed  
✅ Simpler configuration  
✅ Automatic deployments on push  
✅ Works immediately  

Just enable Pages in Settings → Pages → Deploy from branch → main → /web-frontend

Your site will be live at: **https://adhz33.github.io/Off-Highway-Guardian/**

---

**Need help?** Check the [GitHub Pages documentation](https://docs.github.com/en/pages)
