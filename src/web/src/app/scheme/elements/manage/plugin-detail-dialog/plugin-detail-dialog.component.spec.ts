import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PluginDetailDialogComponent } from './plugin-detail-dialog.component';

describe('PluginDetailDialogComponent', () => {
  let component: PluginDetailDialogComponent;
  let fixture: ComponentFixture<PluginDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ PluginDetailDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(PluginDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
